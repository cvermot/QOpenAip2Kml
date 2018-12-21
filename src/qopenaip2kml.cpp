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
    adh = new AeronauticalDataHandler();
    appdh =  new AppDataHandler();
    errorLevel = TErrorLevel_NOERROR;

    createWindow();

    setWindowIcon(QIcon("./img/icon.png"));


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
    hLayout = new QHBoxLayout(this);
    vLeftLayout = new QVBoxLayout(this);
    hLayout->addLayout(vLeftLayout);
    QWidget *window = new QWidget(this);
    mainLayout->addLayout(hLayout);
    window->setLayout(mainLayout);
    setCentralWidget(window);


    //---------------------files management---------------------//
    QGroupBox *fileGroupBox = new QGroupBox(tr("Files"), this);
    QFormLayout *filesFormLayout = new QFormLayout(this);
    fileGroupBox->setLayout(filesFormLayout);
    vLeftLayout->addWidget(fileGroupBox);


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

    createZoneGroup();

    //---------------------Airspaces colors---------------------//
    QGroupBox *colorsGroupBox = new QGroupBox(tr("Airspaces colors"), this);
    QFormLayout *colorsFormLayout = new QFormLayout(this);
    colorsGroupBox->setLayout(colorsFormLayout);
    hLayout->addWidget(colorsGroupBox);

    //classCheckBox = new QVector<QCheckBox*>(airspacesName.length()) ;
    //classPushButton = new QVector<QPushButton*>(airspacesName.length()) ;
    //classCheckBox.reserve(airspacesName.length());
    //classPushButton.reserve(airspacesName.length());
    for(int k = 0 ; k < airspacesName.length() ; k++)
    {
        QHBoxLayout *classLayout = new QHBoxLayout(this);
        QPushButton *pushButton = new QPushButton("Select color", this);
        QCheckBox *checkbox = new QCheckBox(this);
        classPushButton.append(pushButton);
        classCheckBox.append(checkbox);
        classCheckBox[k]->setChecked(true);
        //classCheckBox[k]->setToolTip(airspacesName.at(k).append(" rendered in outputed file"));
        classLayout->addWidget(classPushButton[k]);
        classLayout->addWidget(classCheckBox[k]);
        colorsFormLayout->addRow(airspacesName.at(k), classLayout);
        connect(classPushButton[k], SIGNAL(clicked()), this, SLOT(setColor()));

        //set color to button
        QPalette palette = pushButton->palette();
        QString colorName = appdh->getZonesColors().at(k);
        palette.setColor(QPalette::ButtonText, QColor(colorName));
        pushButton->setPalette(palette);
        pushButton->setText(colorName);
    }

    createSelectAera();

    //---------------------Run button---------------------//
    runPushButton = new QPushButton(tr("Run"), this);
    connect(runPushButton, SIGNAL(clicked(bool)), this, SLOT(runConversion()));
    mainLayout->addWidget(runPushButton);

    progressBar = new QProgressBar();
    statusBar()->addPermanentWidget(progressBar);

}

void QOpenAip2Kml::createZoneGroup()
{
    QGroupBox *zoneGroupBox = new QGroupBox(tr("Zones parameters"), this);
    QFormLayout *zoneFormLayout = new QFormLayout(this);
    zoneGroupBox->setLayout(zoneFormLayout);
    vLeftLayout->addWidget(zoneGroupBox);

    lineThicknessSlider = new QSlider(Qt::Horizontal, this);
    lineThicknessSlider->setRange(0,10);
    lineThicknessSlider->setValue(1);
    lineThicknessSlider->setTickInterval(1);
    lineTicknessLineEdit = new QLineEdit(this);
    lineTicknessLineEdit->setFixedWidth(30);
    lineTicknessLineEdit->setText("1");
    QHBoxLayout *ticknessLayout = new QHBoxLayout(this);
    ticknessLayout->addWidget(lineThicknessSlider);
    ticknessLayout->addWidget(lineTicknessLineEdit);
    zoneFormLayout->addRow("Line tickness", ticknessLayout);

    connect(lineThicknessSlider, SIGNAL(valueChanged(int)), this, SLOT(updateLineTicknessLineEdit(int)));

    transparencySlider = new QSlider(Qt::Horizontal, this);
    transparencySlider->setRange(0,100);
    transparencySlider->setValue(15);
    transparencySlider->setTickInterval(10);
    transparencyLineEdit = new QLineEdit(this);
    transparencyLineEdit->setFixedWidth(30);
    transparencyLineEdit->setText("10");
    QHBoxLayout *transparencyLayout = new QHBoxLayout(this);
    transparencyLayout->addWidget(transparencySlider);
    transparencyLayout->addWidget(transparencyLineEdit);
    zoneFormLayout->addRow("Transparency", transparencyLayout);

    connect(transparencySlider, SIGNAL(valueChanged(int)), this, SLOT(updateTransparencyLineEdit(int)));

}

void QOpenAip2Kml::createSelectAera()
{
    QGroupBox *aeraGroupBox = new QGroupBox(tr("Rendered aera"), this);
    QFormLayout *aeraFormLayout = new QFormLayout(this);
    aeraGroupBox->setLayout(aeraFormLayout);
    vLeftLayout->addWidget(aeraGroupBox);

    areaLatMinLineEdit = new QLineEdit("-90", this);
    areaLatMaxLineEdit = new QLineEdit("90", this);
    areaLonMinLineEdit = new QLineEdit("-180", this);
    areaLonMaxLineEdit = new QLineEdit("180", this);

    aeraFormLayout->addRow(tr("Minimum latitude"), areaLatMinLineEdit);
    aeraFormLayout->addRow(tr("Maximum latitude"), areaLatMaxLineEdit);
    aeraFormLayout->addRow(tr("Minimum longitude"), areaLonMinLineEdit);
    aeraFormLayout->addRow(tr("Minimum longitude"), areaLonMaxLineEdit);

}

void QOpenAip2Kml::updateLineTicknessLineEdit(int p_value)
{
    lineTicknessLineEdit->setText(QString::number(p_value));
}

void QOpenAip2Kml::updateTransparencyLineEdit(int p_value)
{
    transparencyLineEdit->setText(QString::number(p_value));
}

void QOpenAip2Kml::setColor()
{
    QPushButton *button = (QPushButton *)sender();
    QColor color;
    color = QColorDialog::getColor(Qt::green, this);
    if (color.isValid())
    {
        QPalette palette = button->palette();
        palette.setColor(QPalette::ButtonText, color);
        button->setPalette(palette);
        button->setText(color.name());
    }
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
    collectParameters();

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

void QOpenAip2Kml::collectParameters()
{
    QString colorValue = "";
    for(int k = 0 ; k < TAirspaceCategory__INTERNAL_NUMBER_OF_VALUE ; k++)
    {
        colorValue = (classPushButton.at(k)->text()).remove(QRegExp("[^a-zA-Z\\d\\s]"));
        appdh->setZoneColor(static_cast<TAirspaceCategory>(k), colorValue);
        appdh->setZoneActivated(static_cast<TAirspaceCategory>(k), classCheckBox.at(k)->isChecked());
    }
    appdh->setTransparency(transparencySlider->value());
    appdh->setLineThickness(lineThicknessSlider->value());
    appdh->setArea(areaLatMinLineEdit->text().toDouble(),
                   areaLatMaxLineEdit->text().toDouble(),
                   areaLonMinLineEdit->text().toDouble(),
                   areaLonMaxLineEdit->text().toDouble());

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

         KmlWriter *kw = new KmlWriter(adh, appdh, QString(outputFileLineEdit->text()));
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
    adh->flushDatas();

}

void QOpenAip2Kml::about()
{
    QMessageBox::about(this, tr("About QOpenAip2Kml"),
            tr("<b>QOpenAip2Kml v 1.1</b> < br />< br /> "
               "A software aimed to convert AIP files to KML file. <br />"
               "You can download updates on the official website of the project"
               " <a href=\"http://afterflight.org\">http://afterflight.org</a>.<br />< br/>"
               "You can download AIP files on <a href=\"http://openaip.net\">http://openaip.net</a>.<br />< br/>"
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
