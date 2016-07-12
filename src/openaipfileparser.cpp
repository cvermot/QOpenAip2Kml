/*   This software is aimed to convert AIP files to KML file.
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "openaipfileparser.h"

OpenAipFileParser::OpenAipFileParser(AeronauticalDataHandler *const p_adh,
                                     const QString p_filename,
                                     QObject *parent) : QObject(parent)
{
    adh = p_adh;
    filename = p_filename;
}

//méthode de détéction du type de fichier
void OpenAipFileParser::parseOpenAipFile()
{
    //AeronauticalDataHandler p_adh;
    QDomDocument *OAipDoc = new QDomDocument();
        QFile *OAipFile = new QFile(filename);
        OAipFile->open(QIODevice::ReadOnly);
        if(OAipFile->isOpen() == false)
        {
            OAipFile->close();
            qCritical() << "Unable to open file" << filename <<". File didn't exist or is not readable.";
            emit errorOccured(TErrorLevel_STOP, tr("OpenAIP file opening"), tr("<b>Unable to open file</b><br /> Please make sure the file is present and readable."));
        }
        else
        {
            //caculate number of lines in file in order to have some metrics to give progress to user
            //Qt didn't give any method for this purpose : use the old school method !
            int nbLines = 0;
            while( !OAipFile->atEnd())
            {
                OAipFile->readLine();
                nbLines++;
            }
            OAipFile->reset();
            emit elementsToBeProcessed(nbLines);

            #if QT_VERSION >= 0x050500
            //QInfo introduced by Qt 5.5
            qInfo() << "File" << filename << "open. Contain" << nbLines << "lines.";
            #endif
            OAipDoc->setContent(OAipFile);
            OAipFile->close();

            QDomElement oaip=OAipDoc->documentElement();
            if(oaip.tagName().toLower() != "openaip")
            {
                qCritical() << "This file is not a valid openAIP file.";
                emit errorOccured(TErrorLevel_STOP, tr("OpenAIP file opening"), tr("<b>This is not a valid OpenAip file</b><br />"));
            }
            else if(oaip.attribute("DATAFORMAT") != "1.1")
            {
                qWarning() << "The opened file is not an OpenAip 1.1 file. This application is designed to decode files in version 1.1";
                emit errorOccured(TErrorLevel_CONTINUE, "OpenAip file format", "Warning, the opened file is not an OpenAip 1.1 file. <br />This application is designed to decode files in version 1.1.<br />The application is trying to decode the file but some elements may be ignored.");
            }
            #if QT_VERSION >= 0x050500
            //QInfo introduced by Qt 5.5
            qInfo() << "File successfully opened.";
            #endif

            QDomElement type=oaip.firstChild().toElement();
            while(!type.isNull())
            {
                if (type.tagName().toLower() == "airspaces")
                {
                    #if QT_VERSION >= 0x050500
                    //QInfo introduced by Qt 5.5
                    qInfo() << "Airspace file detected";
                    #endif
                    QDomElement airspaces=type.firstChild().toElement();
                    parseAirspaces(*adh, airspaces);
                }
                else if (type.tagName().toLower() == "waypoints")
                {
                    #if QT_VERSION >= 0x050500
                    //QInfo introduced by Qt 5.5
                    qInfo() << "Airport file detected";
                    #endif
                    qCritical() << "Fileformat not yet supported";
                    QDomElement airports=type.firstChild().toElement();
                    //parseAirport(*adh, airports);
                }
                else if (type.tagName().toLower() == "navaids")
                {
                    #if QT_VERSION >= 0x050500
                    //QInfo introduced by Qt 5.5
                    qInfo() << "Navaids file detected";
                    #endif
                    qCritical() << "Fileformat not yet supported";
                    QDomElement navaids=type.firstChild().toElement();
                    //parseNavaids(*adh, navaids);
                }
                else if (type.tagName().toLower() == "hotspots")
                {
                    #if QT_VERSION >= 0x050500
                    //QInfo introduced by Qt 5.5
                    qInfo() << "Hotspots file detected";
                    #endif
                    qCritical() << "No plans to support this format. Please report needs (if any !) to autor.";
                }
                type = type.nextSibling().toElement();
            }
        }

        delete(OAipDoc);
        delete(OAipFile);

        emit finished();
}

//methode pour parcourir chaque ASP un à un
void OpenAipFileParser::parseAirspaces(AeronauticalDataHandler &p_adh,
                                       QDomElement p_airspaces)
{
    while(!p_airspaces.isNull())
    {
        if (p_airspaces.tagName().toLower() == "asp")
        {
            //QDomElement anAirspace=airspaces.firstChild().toElement();
            //qDebug() << airspaces.attribute("CATEGORY");
            parseAirspace(p_adh, p_airspaces);
            emit progress(p_airspaces.lineNumber());
        }
        p_airspaces = p_airspaces.nextSibling().toElement();
    }
}

//parse un espace aerien
void OpenAipFileParser::parseAirspace(AeronauticalDataHandler &p_adh,
                                      QDomElement p_airspaces)
{
    TAirspace airspace = AIRSPACE_INIT;

    QDomElement airspaceDomElement=p_airspaces.firstChild().toElement();
    airspace.category = convertAirspaceCategory(p_airspaces.attribute("CATEGORY").toUpper());

    while(!airspaceDomElement.isNull())
    {
        //gestion des sous elements
        if(airspaceDomElement.tagName().toLower() == "name")
        {
            airspace.name = airspaceDomElement.text();
        }
        if(airspaceDomElement.tagName().toLower() == "altlimit_top")
        {
            airspace.topAltitude = parseAltitudeInfo(airspaceDomElement);
        }
        if(airspaceDomElement.tagName().toLower() == "altlimit_bottom")
        {
            airspace.bottomAltitude = parseAltitudeInfo(airspaceDomElement);
        }
        if(airspaceDomElement.tagName().toLower() == "geometry")
        {
            airspace.geometry = parsePolygon(airspaceDomElement);
            airspace.latLonLst = parsePolygonLatLon(airspaceDomElement);
        }
        airspaceDomElement = airspaceDomElement.nextSibling().toElement();
    }
    p_adh.addAirspace(airspace);
    //qDebug() << "Un espace aérien" << airspace.category;
    airspace = AIRSPACE_INIT;
}

TAirspaceAltitude OpenAipFileParser::parseAltitudeInfo(const QDomElement p_altitudeDomElement)
{
    TAirspaceAltitude airspaceAltitude = AIRSPACE_ALTITUDE_INIT;

    if(p_altitudeDomElement.attribute("REFERENCE").toUpper() == "STD")
    {
        airspaceAltitude.reference = TAirspaceAltitudeReference_STD;
    }
    else if(p_altitudeDomElement.attribute("REFERENCE").toUpper() == "GND")
    {
        airspaceAltitude.reference = TAirspaceAltitudeReference_GND;
    }
    else if(p_altitudeDomElement.attribute("REFERENCE").toUpper() == "MSL")
    {
        airspaceAltitude.reference = TAirspaceAltitudeReference_MSL;
    }
    else
    {
        airspaceAltitude.reference = TAirspaceAltitudeReference_UNDEF;
        qWarning() << "Altitude reference undefined";
    }

    QDomElement altTopDomElement = p_altitudeDomElement.firstChild().toElement();
    if(altTopDomElement.tagName().toLower() == "alt")
    {
        if(altTopDomElement.attribute("UNIT") == "F")
        {
            airspaceAltitude.unit = TAltitudeUnit_FT;
        }
        else if(altTopDomElement.attribute("UNIT") == "FL")
        {
            airspaceAltitude.unit = TAltitudeUnit_FL;
        }
        else
        {
            airspaceAltitude.unit = TAltitudeUnit_UNDEF;
            qWarning() << "Altitude unit undefined";
        }
        airspaceAltitude.value = p_altitudeDomElement.text().toInt();
    }

    return airspaceAltitude;

}

QString OpenAipFileParser::parsePolygon(const QDomElement p_polygonDomElement)
{
    QString out;
    out = p_polygonDomElement.text().replace(" ",";").replace(",", " ").replace(";", ",");
    return out;
}

QVector<TLatLon> OpenAipFileParser::parsePolygonLatLon(const QDomElement p_polygonDomElement)
{
    QVector<TLatLon> sortie;
    QStringList out = p_polygonDomElement.text().split(", ");
    for(int k = 0 ; k < out.size() ; k++)
    {
        QStringList latLng = out.value(k).split(" ");
        TLatLon latLon;
        latLon.longitude = latLng.at(0).toDouble();
        latLon.latitude = latLng.at(1).toDouble();
        sortie.append(latLon);
    }

    return sortie;
}

TAirspaceCategory OpenAipFileParser::convertAirspaceCategory(const QString p_category)
{
    TAirspaceCategory category = TAirspaceCategory_OTHER;

    if(p_category == "A")
    {
        category = TAirspaceCategory_A;
    }
    else if(p_category == "B")
    {
        category = TAirspaceCategory_B;
    }
    else if(p_category == "C")
    {
        category = TAirspaceCategory_C;
    }
    else if(p_category == "D")
    {
        category = TAirspaceCategory_D;
    }
    else if(p_category == "E")
    {
        category = TAirspaceCategory_E;
    }
    else if(p_category == "F")
    {
        category = TAirspaceCategory_F;
    }
    else if(p_category == "G")
    {
        category = TAirspaceCategory_G;
    }
    else if(p_category == "DANGER")
    {
        category = TAirspaceCategory_DANGER;
    }
    else if(p_category == "GLIDING")
    {
        category = TAirspaceCategory_GLIDING;
    }
    else if(p_category == "RESTRICTED")
    {
        category = TAirspaceCategory_RESTRICTED;
    }
    else if(p_category == "PROHIBITED")
    {
        category = TAirspaceCategory_PROHIBITED;
    }
    else if(p_category == "CTR")
    {
        category = TAirspaceCategory_CTR;
    }
    else if(p_category == "TMA")
    {
        category = TAirspaceCategory_TMA;
    }
    else if(p_category == "TMZ")
    {
        category = TAirspaceCategory_TMZ;
    }
    else if(p_category == "WAVE")
    {
        category = TAirspaceCategory_WAVE;
    }
    else if(p_category == "FIR")
    {
        category = TAirspaceCategory_FIR;
    }
    else if(p_category == "UIR")
    {
        category = TAirspaceCategory_UIR;
    }
    else if(p_category == "RMZ")
    {
        category = TAirspaceCategory_RMZ;
    }
    else
    {
        //TAirspaceCategory_OTHER
        qWarning() << "Airspace category undefined";
    }
    return category;

}
