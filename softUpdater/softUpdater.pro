#-------------------------------------------------
#
# Project created by QtCreator 2013-09-23T15:27:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = softUpdater
TEMPLATE = app


SOURCES += src/main.cpp\
        src/MainWindow.cpp \
    src/detailsParser.cpp \
    src/xmlDataParser.cpp

HEADERS  += src/MainWindow.h \
    src/detailsParser.h \
    src/xmlDataParser.h

FORMS    += src/MainWindow.ui
