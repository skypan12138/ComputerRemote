#-------------------------------------------------
#
# Project created by QtCreator 2016-07-23T16:37:40
#
#-------------------------------------------------

QT       += core gui serialport
RC_ICONS = logo.ico
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Com
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
