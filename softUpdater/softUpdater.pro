#-------------------------------------------------
#
# Project created by QtCreator 2013-09-23T15:27:43
#
#-------------------------------------------------

QT       += core network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += console

TARGET = softUpdater
TEMPLATE = app


SOURCES += src/main.cpp\
    src/detailsParser.cpp \
    src/xmlDataParser.cpp \
    src/downloader.cpp \
    src/communicator.cpp \
    src/update.cpp \
    src/updateProcessor.cpp \
    src/updateStorage.cpp

HEADERS  += \
    src/detailsParser.h \
    src/xmlDataParser.h \
    src/downloader.h \
    src/communicator.h \
    src/update.h \
    src/updateProcessor.h \
    src/updateStorage.h

