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

#include "qopenaip2kml.h"
#include <QApplication>
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QLibraryInfo>
#include <QDateTime>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    QString txt;
    switch (type) {
    case QtDebugMsg:
        txt = QString("DEBUG:    ");
        break;
    #if QT_VERSION >= 0x050500
    //QInfo introduced by Qt 5.5
    case QtInfoMsg:
        txt = QString("INFO:     ");
        break;
    #endif
    case QtWarningMsg:
        txt = QString("WARNING:  ");
        break;
    case QtCriticalMsg:
        txt = QString("CRITICAL: ");
        break;
    case QtFatalMsg:
        txt = QString("FATAL:    ");
        break;
    default:
        txt = QString("OTHER:    ");
        break;
    }
    //if debugging, datas context.file // context.line // context.function are avalable

    QFile outFile("log.txt");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append | QFile::Text);
    QTextStream ts(&outFile);
    ts << txt << QDateTime::currentDateTime().toString(Qt::ISODate) << " " <<  localMsg.constData() << context.file << context.line << context.function << endl;

     if(type == QtFatalMsg)
     {
         abort();
     }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(!QLibraryInfo::isDebugBuild())
    {
        qInstallMessageHandler(myMessageOutput);
    }
    QOpenAip2Kml w;
    w.show();

    return a.exec();
}
