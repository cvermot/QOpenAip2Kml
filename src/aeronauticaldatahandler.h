/*  This software is aimed to convert AIP files to KML file.
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

#ifndef AERONAUTICALDATAHANDLER_H
#define AERONAUTICALDATAHANDLER_H

#include <QObject>
#include <QVector>
#include <QDebug>
#include <qopenaip2kmltypes.h>

class AeronauticalDataHandler : public QObject
{
    Q_OBJECT
public:
    explicit AeronauticalDataHandler(QObject *parent = 0);
    ~AeronauticalDataHandler();
    void addAirspace(TAirspace p_airspace);
    void clean();
    const QVector<TAirspace> getAirspaces();
    QVector<TAirspace> getAirspace(TAirspaceCategory p_category);
    static QString getGeometryAsString(QVector<TLatLon> p_latLon, int p_altitude);
    static QString getGeometryAsStringWithoutAltitude(QVector<TLatLon> p_latLon);
    void flushDatas();

signals:

public slots:

private:
    QVector<TAirspace> airspaces;
};

#endif // AERONAUTICALDATAHANDLER_H
