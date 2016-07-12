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

#ifndef QOPENAIP2KMLTYPES_H
#define QOPENAIP2KMLTYPES_H

#include <QString>
#include <QVector>
#include <QObject>

enum TAirspaceAltitudeReference
{
    TAirspaceAltitudeReference_UNDEF,
    TAirspaceAltitudeReference_GND,
    TAirspaceAltitudeReference_MSL,
    TAirspaceAltitudeReference_STD
};

enum TAltitudeUnit
{
    TAltitudeUnit_UNDEF,
    TAltitudeUnit_FL,
    TAltitudeUnit_FT
};

enum TAirspaceCategory
{
    TAirspaceCategory_OTHER,
    TAirspaceCategory_A,
    TAirspaceCategory_B,
    TAirspaceCategory_C,
    TAirspaceCategory_D,
    TAirspaceCategory_E,
    TAirspaceCategory_F,
    TAirspaceCategory_G,
    TAirspaceCategory_DANGER,
    TAirspaceCategory_GLIDING,
    TAirspaceCategory_RESTRICTED,
    TAirspaceCategory_PROHIBITED,
    TAirspaceCategory_CTR,
    TAirspaceCategory_TMA,
    TAirspaceCategory_TMZ,
    TAirspaceCategory_WAVE,
    TAirspaceCategory_FIR,
    TAirspaceCategory_UIR,
    TAirspaceCategory_RMZ,
    TAirspaceCategory__INTERNAL_NUMBER_OF_VALUE
};

static const QVector<QString> airspacesName = QVector<QString>()
                                                                 << "Other"
                                                                 << "A Class"
                                                                 << "B Class"
                                                                 << "C Class"
                                                                 << "D Class"
                                                                 << "E Class"
                                                                 << "F Class"
                                                                 << "G Class"
                                                                 << "Danger"
                                                                 << "Gliding"
                                                                 << "Restricted"
                                                                 << "Prohibited"
                                                                 << "CTR"
                                                                 << "TMA"
                                                                 << "TMZ"
                                                                 << "WAVE"
                                                                 << "FIR"
                                                                 << "UIR"
                                                                 << "RMZ";

struct TAirspaceAltitude
{
    TAirspaceAltitudeReference reference;
    TAltitudeUnit unit;
    int value;
};
static const TAirspaceAltitude AIRSPACE_ALTITUDE_INIT =
{
    TAirspaceAltitudeReference_UNDEF,
    TAltitudeUnit_UNDEF,
    0
};

struct TLatLon
{
    double latitude;
    double longitude;
};

struct TAirspace
{
    QString name;
    QString frequency;
    TAirspaceAltitude bottomAltitude;
    TAirspaceAltitude topAltitude;
    TAirspaceCategory category;
    QString geometry;
    QVector<TLatLon> latLonLst;
};

static const TAirspace AIRSPACE_INIT =
{
    QString(""),
    QString(""),
    AIRSPACE_ALTITUDE_INIT,
    AIRSPACE_ALTITUDE_INIT,
    TAirspaceCategory_OTHER,
    QString(""),
    QVector<TLatLon>()
};

class QOpenAip2KmlTypes : public QObject
{
    Q_OBJECT
public:
    //explicit AppDataHandler(QObject *parent = 0);

signals:

public slots:

};


#endif // QOPENAIP2KMLTYPES_H
