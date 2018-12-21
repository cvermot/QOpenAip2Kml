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
#include <QSlider>
#include <QFormLayout>
#include <QMessageBox>
#include <QProgressBar>
#include <QDebug>
#include <QThread>
#include <QIcon>
#include <QLibraryInfo>
#include <QMenuBar>
#include <QToolBar>
#include <QCheckBox>
#include <QColorDialog>
#include <QStatusBar>
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
    AppDataHandler *appdh;

private:
    void createWindow();
    void createZoneGroup();
    void createSelectAera();
    void parseOpenAipFile();
    void collectParameters();

    QMenu *fileMenu;
    QMenu *helpMenu;
    QAction *aboutQtAction;
    QAction *aboutAction;
    QVBoxLayout *mainLayout;
    QHBoxLayout *hLayout;
    QVBoxLayout *vLeftLayout;

    TErrorLevel errorLevel;

    //liste des espaces aeriens
    QLineEdit *airspaceFileLineEdit;
    //liste des aeroports
    QLineEdit *airportFileLineEdit;
    //elements de radionav
    QLineEdit *navAidsFileLineEdit;
    //fichier de sortie
    QLineEdit *outputFileLineEdit;

    QLineEdit *areaLatMinLineEdit;
    QLineEdit *areaLonMinLineEdit;
    QLineEdit *areaLatMaxLineEdit;
    QLineEdit *areaLonMaxLineEdit;

    QPushButton *runPushButton;

    QProgressBar *progressBar;

    QVector<QCheckBox*> classCheckBox;
    QVector<QPushButton*> classPushButton;

    QSlider *lineThicknessSlider;
    QLineEdit *lineTicknessLineEdit;

    QSlider *transparencySlider;
    QLineEdit *transparencyLineEdit;


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
    void setColor();
    void updateLineTicknessLineEdit(int p_value);
    void updateTransparencyLineEdit(int p_value);

 signals:
    void requestQMessageBox(int errorLevel, QString windowTitle, QString errorDescription);

};

#endif // QOPENAIP2KML_H
