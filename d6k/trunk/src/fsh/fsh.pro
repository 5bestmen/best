#-------------------------------------------------
#
# Project created by QtCreator 2016-09-01T19:48:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fsh
TEMPLATE = app
DESTDIR = ../../bin

SOURCES += main.cpp\
        dialog.cpp

HEADERS  += dialog.h

FORMS    += dialog.ui

RESOURCES += fsh.qrc

win32:RC_FILE = fsh.rc