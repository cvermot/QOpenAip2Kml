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

#ifndef APPDATAHANDLER_H
#define APPDATAHANDLER_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QDebug>
#include <qopenaip2kmltypes.h>

enum TErrorLevel
{
    TErrorLevel_NOERROR,
    TErrorLevel_CONTINUE,
    TErrorLevel_STOP
};

class AppDataHandler : public QObject
{   
    Q_OBJECT
public:
    explicit AppDataHandler(QObject *parent = 0);

    QVector<QString> getZonesColors();
    QVector<bool> getZonesActivations();
    QString getTransparency();
    int getLineThickness();
    void setZoneColor (const TAirspaceCategory category, const QString colorCode);
    void setZoneActivated (const TAirspaceCategory category, const bool zoneIsActivated);
    void setTransparency(int p_transparency);
    void setLineThickness(int p_lineThickness);
    void setArea(double p_latMin, double p_latMax, double p_lonMin, double p_lonMax);

private:
    QVector<QString> zonesColors;
    QVector<bool> zonesActivations;
    int transparency;
    int lineThickness;
    double latMin;
    double latMax;
    double lonMin;
    double lonMax;

signals:

public slots:

};

#endif // APPDATAHANDLER_H
