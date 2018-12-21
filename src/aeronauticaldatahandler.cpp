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

#include "aeronauticaldatahandler.h"

AeronauticalDataHandler::AeronauticalDataHandler(QObject *parent) : QObject(parent)
{

}

AeronauticalDataHandler::~AeronauticalDataHandler()
{

}

void AeronauticalDataHandler::addAirspace(TAirspace p_airspace)
{  
    airspaces.append(p_airspace);
}

void AeronauticalDataHandler::flushDatas()
{
    //TODO : doing some stuff in order to effectively clear the memory from Qt 5.7
    airspaces.clear();
}

const QVector<TAirspace> AeronauticalDataHandler::getAirspaces()
{
    return airspaces;
}

QVector<TAirspace> AeronauticalDataHandler::getAirspace(TAirspaceCategory p_category)
{
    QVector<TAirspace> airspaceByCategory;

    for(int k = 0; k < airspaces.size() ; k++)
    {
        if(airspaces.at(k).category == p_category)
        {
            airspaceByCategory.append(airspaces.at(k));
        }

    }
    return airspaceByCategory;
}

QString AeronauticalDataHandler::getGeometryAsString(QVector<TLatLon> p_latLon, int p_altitude)
{
    QString geometry;

    for(int k = 0 ; k < p_latLon.size() ; k++)
    {
        geometry.append(QString::number(p_latLon.at(k).longitude));
        geometry.append(",");
        geometry.append(QString::number(p_latLon.at(k).latitude));
        geometry.append(",");
        geometry.append(QString::number(p_altitude));
        geometry.append(" ");
    }

    return geometry;
}

QString AeronauticalDataHandler::getGeometryAsStringWithoutAltitude(QVector<TLatLon> p_latLon)
{
    QString geometry;

    for(int k = 0 ; k < p_latLon.size() ; k++)
    {
        geometry.append(QString::number(p_latLon.at(k).longitude));
        geometry.append(",");
        geometry.append(QString::number(p_latLon.at(k).latitude));
        geometry.append(" ");
    }

    return geometry;
}

void AeronauticalDataHandler::clean()
{
    airspaces.clear();
}


