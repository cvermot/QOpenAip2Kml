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

#include "appdatahandler.h"

AppDataHandler::AppDataHandler(QObject *parent) : QObject(parent)
{
    //ff0000 = blue
    //0000ff = red
    //00aa00 = green

    //default is blue and active
    zonesColors.fill("#ff0000", TAirspaceCategory__INTERNAL_NUMBER_OF_VALUE);
    zonesActivations.fill(true, TAirspaceCategory__INTERNAL_NUMBER_OF_VALUE);

    //tunning some zone's colors
    zonesColors.replace(TAirspaceCategory_A, "#0000ff");
    zonesColors.replace(TAirspaceCategory_DANGER, "#0000ff");
    zonesColors.replace(TAirspaceCategory_RESTRICTED, "#0000ff");
    zonesColors.replace(TAirspaceCategory_PROHIBITED, "#0000ff");
    zonesColors.replace(TAirspaceCategory_FIR, "#00aa00");

    transparency = 15;
    lineThickness = 1;

    latMin = -90;
    latMax = 90;
    lonMin = -180;
    lonMax = 180;
}

QVector<QString> AppDataHandler::getZonesColors()
{
    return zonesColors;
}

QVector<bool> AppDataHandler::getZonesActivations()
{
    return zonesActivations;
}

QString AppDataHandler::getTransparency()
{
    return QString::number(transparency*255/100, 16);
}

int AppDataHandler::getLineThickness()
{
    return lineThickness;
}

void AppDataHandler::setZoneColor (const TAirspaceCategory category, const QString colorCode)
{
    zonesColors.replace(category, colorCode);
}

void AppDataHandler::setZoneActivated (const TAirspaceCategory category, const bool zoneIsActivated)
{
    zonesActivations.replace(category, zoneIsActivated);
}

void AppDataHandler::setTransparency(int p_transparency)
{
    transparency = p_transparency;
}

void AppDataHandler::setLineThickness(int p_lineThickness)
{
    lineThickness = p_lineThickness;
}

void AppDataHandler::setArea(double p_latMin, double p_latMax, double p_lonMin, double p_lonMax)
{
    latMin = p_latMin;
    latMax = p_latMax;
    lonMin = p_lonMin;
    lonMax = p_lonMax;
}
