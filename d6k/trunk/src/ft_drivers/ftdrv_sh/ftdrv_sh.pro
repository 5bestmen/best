#-------------------------------------------------
#
# Project created by QtCreator 2016-09-01T19:48:38
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ftdrv_sh
TEMPLATE = app
DESTDIR = ../../../bin

SOURCES += main.cpp\
        dialog.cpp

HEADERS  += dialog.h

FORMS    += dialog.ui

RESOURCES += ftdrv_sh.qrc

win32:RC_FILE = ftdrv_sh.rc

LIBS += -L"./../../lib" \
    -l../../lib/log \
  