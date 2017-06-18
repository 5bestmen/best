#-------------------------------------------------
#
# Project created by QtCreator 2016-09-01T19:48:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = model
TEMPLATE = app
DESTDIR = ../../bin

SOURCES += main.cpp\
        model.cpp

HEADERS  += model.h

#FORMS    += dialog.ui

RESOURCES += model.qrc

win32:RC_FILE = model.rc