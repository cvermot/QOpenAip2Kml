#-------------------------------------------------
#
# Project created by QtCreator 2016-02-21T13:30:58
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QOpenAip2Kml
TEMPLATE = app


SOURCES += main.cpp\
        qopenaip2kml.cpp \
    aeronauticaldatahandler.cpp \
    openaipfileparser.cpp \
    kmlwriter.cpp \
    appdatahandler.cpp \
    qopenaip2kmltypes.cpp

HEADERS  += qopenaip2kml.h \
    qopenaip2kmltypes.h \
    aeronauticaldatahandler.h \
    openaipfileparser.h \
    kmlwriter.h \
    appdatahandler.h

FORMS    += qopenaip2kml.ui

DISTFILES += \
    qopenaip2kml.rc

RC_FILE = qopenaip2kml.rc
