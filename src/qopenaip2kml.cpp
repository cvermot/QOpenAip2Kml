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
#include "ui_qopenaip2kml.h"

QOpenAip2Kml::QOpenAip2Kml(QWidget *parent) : QMainWindow(parent)
{
    createWindow();

    setWindowIcon(QIcon("./img/icon.png"));

    adh = new AeronauticalDataHandler();
    errorLevel = TErrorLevel_NOERROR;
}

void QOpenAip2Kml::createWindow()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    helpMenu = menuBar()->addMenu(tr("&Help"));
    aboutQtAction = new QAction(tr("&About Qt"), this);
    aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));
    helpMenu->addAction(aboutQtAction);
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    aboutAction = new QAction(tr("About &QOpenAip2Kml"), this);
    aboutAction->setStatusTip(tr("Show this application's About box"));
    helpMenu->addAction(aboutAction);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));


    mainLayout = new QVBoxLayout(this);
    QWidget *window = new QWidget(this);
    window->setLayout(mainLayout);
    setCentralWidget(window);

    //---------------------files management---------------------//
    QGroupBox *fileGroupBox = new QGroupBox(tr("Files"), this);
    QFormLayout *filesFormLayout = new QFormLayout(this);
    fileGroupBox->setLayout(filesFormLayout);
    mainLayout->addWidget(fileGroupBox);

    //---airspaces file---//
    QHBoxLayout *airspaceFileLayout = new QHBoxLayout(this);
    airspaceFileLineEdit = new QLineEdit(this);
    QPushButton *airspaceFilePushButton = new QPushButton(tr("Select"), this);
    airspaceFileLayout->addWidget(airspaceFileLineEdit);
    airspaceFileLayout->addWidget(airspaceFilePushButton);
    connect(airspaceFilePushButton, SIGNAL(clicked(bool)), this, SLOT(selectAirspaceFileName()));
    filesFormLayout->addRow(tr("Airspaces file"), airspaceFileLayout);

    //---airports file---//
    QHBoxLayout *airportFileLayout = new QHBoxLayout(this);
    airportFileLineEdit = new QLineEdit(this);
    QPushButton *airportFilePushButton = new QPushButton(tr("Select"), this);
    airportFilePushButton->setDisabled(true);
    airportFilePushButton->setToolTip(tr("Feature not yet supported"));
    airportFileLineEdit->setDisabled(true);
    airportFileLineEdit->setToolTip(tr("Feature not yet supported"));
    airportFileLayout->addWidget(airportFileLineEdit);
    airportFileLayout->addWidget(airportFilePushButton);
    connect(airportFilePushButton, SIGNAL(clicked(bool)), this, SLOT(selectAirportFileName()));
    filesFormLayout->addRow(tr("Airport file"), airportFileLayout);

    //---navaids file---//
    QHBoxLayout *navAidsFileLayout = new QHBoxLayout(this);
    navAidsFileLineEdit = new QLineEdit(this);
    QPushButton *navAidsFilePushButton = new QPushButton(tr("Select"), this);
    navAidsFilePushButton->setDisabled(true);
    navAidsFilePushButton->setToolTip(tr("Feature not yet supported"));
    navAidsFileLineEdit->setDisabled(true);
    navAidsFileLineEdit->setToolTip(tr("Feature not yet supported"));
    navAidsFileLayout->addWidget(navAidsFileLineEdit);
    navAidsFileLayout->addWidget(navAidsFilePushButton);
    connect(navAidsFilePushButton, SIGNAL(clicked(bool)), this, SLOT(selectNavAidsFileName()));
    filesFormLayout->addRow(tr("Navaids file"), navAidsFileLayout);

    //---output file---//
    QHBoxLayout *outputFileLayout = new QHBoxLayout(this);
    outputFileLineEdit = new QLineEdit(this);
    QPushButton *outputFilePushButton = new QPushButton("Select",this);
    outputFileLayout->addWidget(outputFileLineEdit);
    outputFileLayout->addWidget(outputFilePushButton);
    connect(outputFilePushButton, SIGNAL(clicked(bool)), this, SLOT(selectOutputFileName()));
    filesFormLayout->addRow(tr("Output file"), outputFileLayout);

    if(QLibraryInfo::isDebugBuild())
    {
        airspaceFileLineEdit->setText("../openaip_airspace_france_fr.aip");
        outputFileLineEdit->setText("../out.kml");
    }

    //---------------------Run button---------------------//
    runPushButton = new QPushButton(tr("Run"), this);
    connect(runPushButton, SIGNAL(clicked(bool)), this, SLOT(runConversion()));
    mainLayout->addWidget(runPushButton);

    progressBar = new QProgressBar();
    statusBar()->addPermanentWidget(progressBar);

}

void QOpenAip2Kml::selectAirspaceFileName()
{
    QString filename = QFileDialog::getOpenFileName(0, "Select a file",  QDir::homePath(), tr("OpenAip file (*.aip)"));
    airspaceFileLineEdit->setText(filename);
}

void QOpenAip2Kml::selectAirportFileName()
{
    QString filename = QFileDialog::getOpenFileName(0, "Select a file", QDir::homePath(), tr("OpenAip file (*.aip)"));
    airportFileLineEdit->setText(filename);
}

void QOpenAip2Kml::selectNavAidsFileName()
{
    QString filename = QFileDialog::getOpenFileName(0, "Select a file", QDir::homePath(), tr("OpenAip file (*.aip)"));
    navAidsFileLineEdit->setText(filename);
}

void QOpenAip2Kml::selectOutputFileName()
{
    QString filename = QFileDialog::getSaveFileName(0, "Select a file", QDir::homePath(), tr("KML file (*.kml)"));
    outputFileLineEdit->setText(filename);
}

void QOpenAip2Kml::runConversion()
{
    statusBar()->showMessage(tr("Parsing openAIP files"));

    errorLevel = TErrorLevel_NOERROR;
    adh->clean();

    //disable button to avoid simultaneous run
    runPushButton->setDisabled(true);

    OpenAipFileParser *fp = new OpenAipFileParser(adh, QString(airspaceFileLineEdit->text()));
    QThread* thread = new QThread(this);
    fp->moveToThread(thread);

    connect(thread, SIGNAL(finished()), fp, SLOT(deleteLater()));
    connect(thread, SIGNAL(started()), fp, SLOT(parseOpenAipFile()));
    connect(fp, SIGNAL(progress(int)), progressBar, SLOT(setValue(int)));
    connect(fp, SIGNAL(elementsToBeProcessed(int)), this, SLOT(setMaximum(int)));
    connect(fp, SIGNAL(finished()), this, SLOT(fileParsingIsFinnished()));
    connect(fp, SIGNAL(errorOccured(int, QString, QString)), this, SLOT(displayErrors(int, QString, QString)));

    thread->start();

}

void QOpenAip2Kml::setMaximum(int value)
{
    progressBar->setMaximum(value * 2);
    progressBar->setMinimum(0);
}

void QOpenAip2Kml::displayErrors(int p_errorLevel, QString p_windowTitle, QString p_errorDescription)
{
    if(p_errorLevel == TErrorLevel_CONTINUE)
    {
        errorLevel = TErrorLevel_CONTINUE;
        QMessageBox::information(0, p_windowTitle, p_errorDescription);
    }
    else if(p_errorLevel == TErrorLevel_STOP)
    {
        errorLevel = TErrorLevel_STOP;
        QMessageBox::critical(0, p_windowTitle, p_errorDescription.append(tr("<br />KML file writting stopped")));
        statusBar()->showMessage(tr("Failure : no KML file produced"));
        progressBar->reset();
        runPushButton->setDisabled(false);
    }
}

void QOpenAip2Kml::fileParsingIsFinnished()
{
     #if QT_VERSION >= 0x050500
     //QInfo introduced by Qt 5.5
     qInfo() << "Number of airspaces read" << adh->getAirspaces().length();
     #endif
     if(errorLevel != TErrorLevel_STOP)
     {
         #if QT_VERSION >= 0x050500
         //QInfo introduced by Qt 5.5
         qInfo() << "Beggining KML production";
         #endif
         statusBar()->showMessage(tr("Writing kml file"));

         KmlWriter *kw = new KmlWriter(adh, QString(outputFileLineEdit->text()));
         QThread* thread = new QThread(this);
         kw->moveToThread(thread);

         progressBar->setMaximum(adh->getAirspaces().length()*2);
         progressBar->setMinimum(0);
         progressBar->setValue(adh->getAirspaces().length());

         connect(thread, SIGNAL(finished()), kw, SLOT(deleteLater()));
         connect(thread, SIGNAL(started()), kw, SLOT(writeKml()));
         connect(kw, SIGNAL(progress(int)), this, SLOT(setProgressBarValueWriting(int)));
         connect(kw, SIGNAL(finished()), this, SLOT(fileWritingIsFinnished()));
         connect(kw, SIGNAL(errorOccured(int, QString, QString)), this, SLOT(displayErrors(int, QString, QString)));

         thread->start();
     }
     else
     {
        statusBar()->showMessage(tr("Failure : no KML file produced"));
        progressBar->reset();
        runPushButton->setDisabled(false);
     }

}

void QOpenAip2Kml::setProgressBarValueWriting(int value)
{
    progressBar->setValue(adh->getAirspaces().length() + value);
}

void QOpenAip2Kml::fileWritingIsFinnished()
{
    #if QT_VERSION >= 0x050500
    //QInfo introduced by Qt 5.5
    qInfo() << "File written";
    #endif
    statusBar()->showMessage(tr("Success"), 20000);
    runPushButton->setDisabled(false);

}

void QOpenAip2Kml::about()
{
    QMessageBox::about(this, tr("About QOpenAip2Kml"),
            tr("<b>QOpenAip2Kml v 1.0</b> < br />< br /> "
               "A software aimed to convert AIP files to KML file. <br />"
               "You can download updates on the official website of the project"
               " <a href=\"http://afterflight.org\">http://afterflight.org</a>.<br />< br/>"
               "You can download the AIP files on <a href=\"http://openaip.net\">http://openaip.net</a>.<br />< br/>"
               "Icon by <a href=\"https://www.iconfinder.com/icons/897231/airplane_destination_fly_map_resolutions_travel_vacation_icon\">Laura Reen</a>.< br />< br />< br />"
               " This program is free software: you can redistribute it and/or modify"
               " it under the terms of the GNU General Public License as published by"
               " the Free Software Foundation, either version 3 of the License, or"
               " (at your option) any later version."
               "<br />< br/>"
               "This program is distributed in the hope that it will be useful,"
               " but WITHOUT ANY WARRANTY; without even the implied warranty of"
               " MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the"
               " GNU General Public License for more details."
               "<br />< br/>"
               "You should have received a copy of the GNU General Public License"
               " along with this program.  If not, see <a href=\"http://www.gnu.org/licenses/\">http://www.gnu.org/licenses/</a>."));
}

QOpenAip2Kml::~QOpenAip2Kml()
{

}
