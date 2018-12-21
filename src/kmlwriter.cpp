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

#include "kmlwriter.h"

const QString KmlWriter::K_STYLEMAP_SUFFIX = QString("Map");

KmlWriter::KmlWriter(AeronauticalDataHandler *const p_adh,
                     AppDataHandler *const p_appdh,
                     const QString p_filename,
                     QObject *parent) : QObject(parent)
{
    airspacesProcessedCount = 0;
    adh = p_adh;
    appdh = p_appdh;
    filename = p_filename;

}

void KmlWriter::writeKml()
{
    airspacesProcessedCount = 0;

    //-------------------creation de la structure de base
    //QDomDocument doc("infodoctypeici");
    QDomDocument doc;
    QDomNode xmlNode = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"");
    doc.insertBefore(xmlNode, doc.firstChild());

    QDomElement root = doc.createElement("kml");
    root.setAttribute("xmlns", "http://www.opengis.net/kml/2.2");
    doc.appendChild(root);

    QDomElement document = doc.createElement("Document");
    root.appendChild(document);

    QDomElement snippet = doc.createElement("Snippet");
    document.appendChild(snippet);
    QDomCDATASection snippetText = doc.createCDATASection("created using <a href=\"http://afterflight.org/qopenaip2kml/\"><b>AfterFlight QOpenAip2Kml</b></a>");
    snippet.appendChild(snippetText);

    QDomElement name = doc.createElement("name");
    document.appendChild(name);
    QDomCDATASection nameText = doc.createCDATASection(QString("NomDocument"));
    name.appendChild(nameText);

    createStyles(doc, document);


    //-------------------fin creation de la structure de base

    //-------------------Zones
    for(int k = 0 ; k < airspacesName.length() ; k++)
    {
        if(appdh->getZonesActivations().at(k))
        {
           createZoneByFolder(doc, document, adh->getAirspace(static_cast<TAirspaceCategory>(k)), airspacesName.at(k));
        }
        else
        {
            //update counter in order to reach 100%
            airspacesProcessedCount = airspacesProcessedCount + adh->getAirspace(static_cast<TAirspaceCategory>(k)).size();
        }
    }

    //-------------------fin Zones

    //-------------------actually writting file
    QFile file(filename);
    if(file.open(QIODevice::WriteOnly))
    {
        #if QT_VERSION >= 0x050500
        //QInfo introduced by Qt 5.5
        qInfo() << "output file" << filename << "successfully opened";
        #endif
        QTextStream textStream(&file);
        int indent = 1;
        doc.save(textStream, indent);
    }
    else
    {
        qCritical() << "unable to create output file" << filename;
        emit errorOccured(TErrorLevel_STOP, tr("OpenAIP file opening"), tr("<b>This is not a valid OpenAip file</b><br />"));
    }

    emit finished();
}

void KmlWriter::createZoneByFolder(QDomDocument &p_doc,
                                   QDomElement &p_document,
                                   const QVector<TAirspace> airspaces,
                                   const QString p_name)
{
    QDomElement zonesFolder = p_doc.createElement("Folder");
    p_document.appendChild(zonesFolder);

    QDomElement zonesFolderName = p_doc.createElement("name");
        zonesFolder.appendChild(zonesFolderName);
        QDomText zonesFolderNameValue = p_doc.createTextNode(p_name);
        zonesFolderName.appendChild(zonesFolderNameValue);

    for(int k=0;k<airspaces.count();k++)
    {
            QDomElement placemark = p_doc.createElement("Placemark");
            zonesFolder.appendChild(placemark);

            QDomElement name = p_doc.createElement("name");
            placemark.appendChild(name);
            QDomText nameValue = p_doc.createTextNode(airspaces.at(k).name);
            name.appendChild(nameValue);

            QDomElement styleUrl = p_doc.createElement("styleUrl");
            placemark.appendChild(styleUrl);
            //QDomText styleUrlValue = doc.createTextNode(QString("#Color").append(K_STYLEMAP_SUFFIX));
            QDomText styleUrlValue = p_doc.createTextNode(colorMapZone.at(airspaces.at(k).category));
            styleUrl.appendChild(styleUrlValue);


            QDomElement polygon = p_doc.createElement("Polygon");
            placemark.appendChild(polygon);

            //QDomElement altitudeMode = p_doc.createElement("altitudeMode");
            //polygon.appendChild(altitudeMode);
            //QDomText altitudeModeValue = p_doc.createTextNode("absolute");
            //altitudeMode.appendChild(altitudeModeValue);

            QDomElement outerboundaryis = p_doc.createElement("outerBoundaryIs");
            polygon.appendChild(outerboundaryis);

            QDomElement linearring = p_doc.createElement("LinearRing");
            outerboundaryis.appendChild(linearring);

            QDomElement coordinates = p_doc.createElement("coordinates");
            linearring.appendChild(coordinates);
            //QDomText coordinatesValue = p_doc.createTextNode(airspaces.at(k).geometry);
            //QDomText coordinatesValue = p_doc.createTextNode(AeronauticalDataHandler::getGeometryAsString(airspaces.at(k).latLonLst, airspaces.at(k).bottomAltitude.value));
            QDomText coordinatesValue = p_doc.createTextNode(AeronauticalDataHandler::getGeometryAsStringWithoutAltitude(airspaces.at(k).latLonLst));
            coordinates.appendChild(coordinatesValue);

            //signaling progress
            airspacesProcessedCount++;
            emit progress(airspacesProcessedCount);

    }
}

void KmlWriter::createStyles(QDomDocument &p_doc,
                             QDomElement &p_document)
{
    //initializ the vector with a default value
    colorMapZone.fill("#blueMap", TAirspaceCategory__INTERNAL_NUMBER_OF_VALUE);


    QVector<QString> colors = appdh->getZonesColors();
    for(int k = 0 ; k < airspacesName.length() ; k++)
    {
       QString sanitizedAirspaceName = airspacesName[k];
       sanitizedAirspaceName.remove(QRegExp("[^a-zA-Z\\d\\s]"));
       createStyleWithPair(p_doc, p_document, colors[k], sanitizedAirspaceName);
       colorMapZone.replace(k, QString("#").append(sanitizedAirspaceName).append("Map"));
    }

}

void KmlWriter::createStyleWithPair(QDomDocument &p_doc,
                                    QDomElement &p_document,
                                    const QString p_color,
                                    const QString p_name)
{
    createStyle(p_doc, p_document, p_color, QString("normal").append(p_name));
    createStyle(p_doc, p_document, p_color, QString("hover").append(p_name));
    addStylePair(p_doc, p_document, p_name);
}

void KmlWriter::createStyle(QDomDocument &p_doc,
                            QDomElement &p_document,
                            const QString p_color,
                            const QString p_name)
{

    QDomElement style = p_doc.createElement("Style");
    style.setAttribute("id", p_name);
    p_document.appendChild(style);

    QDomElement lineStyle = p_doc.createElement("LineStyle");
    style.appendChild(lineStyle);

    QDomElement color = p_doc.createElement("color");
    lineStyle.appendChild(color);
    QDomText colorValue = p_doc.createTextNode(QString("ff").append(p_color));
    color.appendChild(colorValue);

    QDomElement width = p_doc.createElement(QString("width"));
    lineStyle.appendChild(width);
    QDomText widthValue = p_doc.createTextNode(QString::number(appdh->getLineThickness()));
    width.appendChild(widthValue);

    QDomElement polyStyle = p_doc.createElement("PolyStyle");
    style.appendChild(polyStyle);

    QDomElement colorpoly = p_doc.createElement("color");
    polyStyle.appendChild(colorpoly);
    QString colorPolyString;
    colorPolyString.append(appdh->getTransparency()).append(p_color);
    QDomText colorpolyValue = p_doc.createTextNode(colorPolyString);
    colorpoly.appendChild(colorpolyValue);
}

void KmlWriter::addStylePair(QDomDocument &p_doc, QDomElement &p_document, QString p_name)
{
    QDomElement styleMap = p_doc.createElement("StyleMap");
    styleMap.setAttribute("id", QString(p_name).append(K_STYLEMAP_SUFFIX));
    p_document.appendChild(styleMap);

    QDomElement pair = p_doc.createElement("Pair");
    styleMap.appendChild(pair);

    QDomElement key = p_doc.createElement("key");
    pair.appendChild(key);
    QDomText keyValue = p_doc.createTextNode("normal");
    key.appendChild(keyValue);

    QDomElement style = p_doc.createElement("styleUrl");
    pair.appendChild(style);
    QDomText styleValue = p_doc.createTextNode(QString("#normal").append(p_name));
    style.appendChild(styleValue);

    QDomElement pair2 = p_doc.createElement("Pair");
    styleMap.appendChild(pair2);

    QDomElement key2 = p_doc.createElement("key");
    pair2.appendChild(key2);
    QDomText keyValue2 = p_doc.createTextNode("highlight");
    key2.appendChild(keyValue2);

    QDomElement style2 = p_doc.createElement("styleUrl");
    pair2.appendChild(style2);
    QDomText styleValue2 = p_doc.createTextNode(QString("#hover").append(p_name));
    style2.appendChild(styleValue2);
}
