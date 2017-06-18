#-------------------------------------------------
#
# Project created by QtCreator 2016-09-01T19:48:38
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = appsh
TEMPLATE = app
DESTDIR = ../../../bin

SOURCES += main.cpp\
        dialog.cpp

HEADERS  += dialog.h

FORMS    += dialog.ui

RESOURCES += appsh.qrc

win32:RC_FILE = appsh.rc

LIBS += -L"./../../lib" \
    -l../../lib/log \
  