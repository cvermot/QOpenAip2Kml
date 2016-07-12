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

#ifndef KMLWRITER_H
#define KMLWRITER_H

#include <QObject>
#include <QDomDocument>
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <qopenaip2kmltypes.h>
#include <aeronauticaldatahandler.h>
#include <appdatahandler.h>


class KmlWriter : public QObject
{
    Q_OBJECT
public:
    explicit KmlWriter(AeronauticalDataHandler *const p_adh, AppDataHandler *const p_appdh, const QString p_filename, QObject *parent = 0);

private:
    void createStyles(QDomDocument &p_doc, QDomElement &p_document);
    void createStyle(QDomDocument &p_doc, QDomElement &p_document, const QString p_color, const QString p_name);
    void createStyleWithPair(QDomDocument &p_doc, QDomElement &p_document, const QString p_color, const QString p_name);
    void addStylePair(QDomDocument &p_doc, QDomElement &p_styleMap, QString p_name);
    void createZoneByFolder(QDomDocument &p_doc, QDomElement &p_document, const QVector<TAirspace> airspaces, const QString p_name);

    QString filename;
    AeronauticalDataHandler *adh;
    AppDataHandler *appdh;
    static const QString K_STYLEMAP_SUFFIX;
    QVector<QString> colorMapZone;
    int airspacesProcessedCount;

signals:
    void progress(int value);
    void finished();
    void errorOccured(int errorLevel, QString windowTitle, QString errorDescription);

public slots:
    void writeKml();
};

#endif // KMLWRITER_H
