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

#ifndef QOPENAIP2KML_H
#define QOPENAIP2KML_H

#include <QMainWindow>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QMessageBox>
#include <QProgressBar>
#include <QDebug>
#include <QThread>
#include <QIcon>
#include <QLibraryInfo>
#include <qopenaip2kmltypes.h>
#include <openaipfileparser.h>
#include <kmlwriter.h>
#include <aeronauticaldatahandler.h>
#include <appdatahandler.h>

class QOpenAip2Kml : public QMainWindow
{
    Q_OBJECT

public:
    explicit QOpenAip2Kml(QWidget *parent = 0);
    ~QOpenAip2Kml();

    AeronauticalDataHandler *adh;

private:
    void createWindow();
    void parseOpenAipFile();

    QMenu *fileMenu;
    QMenu *helpMenu;
    QAction *aboutQtAction;
    QAction *aboutAction;
    QVBoxLayout *mainLayout;

    TErrorLevel errorLevel;

    //liste des espaces aeriens
    QLineEdit *airspaceFileLineEdit;
    //liste des aeroports
    QLineEdit *airportFileLineEdit;
    //elements de radionav
    QLineEdit *navAidsFileLineEdit;
    //fichier de sortie
    QLineEdit *outputFileLineEdit;

    QPushButton *runPushButton;

    QProgressBar *progressBar;

private slots:
    void selectAirspaceFileName();
    void selectAirportFileName();
    void selectNavAidsFileName();
    void selectOutputFileName();
    void runConversion();
    void fileParsingIsFinnished();
    void fileWritingIsFinnished();
    void setMaximum(int value);
    void setProgressBarValueWriting(int value);
    void displayErrors(int p_errorLevel, QString p_windowTitle, QString p_errorDescription);
    void about();

 signals:
    void requestQMessageBox(int errorLevel, QString windowTitle, QString errorDescription);

};

#endif // QOPENAIP2KML_H
