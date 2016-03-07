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

#ifndef OPENAIPFILEPARSER_H
#define OPENAIPFILEPARSER_H

#include <QObject>
#include <QDomDocument>
#include <QMessageBox>
#include <QFile>
#include <QDebug>

#include <qopenaip2kmltypes.h>
#include <aeronauticaldatahandler.h>
#include <appdatahandler.h>

class OpenAipFileParser : public QObject
{
    Q_OBJECT
public:
    explicit OpenAipFileParser(AeronauticalDataHandler *const p_adh, const QString p_filename, QObject *parent = 0);

private:
    void parseAirspaces(AeronauticalDataHandler &p_adh, QDomElement airspaces);
    void parseAirspace(AeronauticalDataHandler &p_adh, QDomElement airspace);
    TAirspaceAltitude parseAltitudeInfo(const QDomElement p_altitudeDomElement);
    TAirspaceCategory convertAirspaceCategory(const QString p_category);
    QString parsePolygon(const QDomElement p_polygonDomElement);
    QVector<TLatLon> parsePolygonLatLon(const QDomElement p_polygonDomElement);

    QString filename;
    AeronauticalDataHandler *adh;

signals:
    void elementsToBeProcessed(int numberOfElements);
    void progress(int value);
    void finished();
    void errorOccured(int errorLevel, QString windowTitle, QString errorDescription);

public slots:
    void parseOpenAipFile();
};

#endif // OPENAIPFILEPARSER_H
