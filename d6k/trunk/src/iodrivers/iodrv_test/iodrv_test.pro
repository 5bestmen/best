#-------------------------------------------------
#
# Project created by QtCreator 2016-09-01T19:48:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = iodrv_test
TEMPLATE = app
DESTDIR = ../../../bin

INCLUDEPATH += ./GeneratedFiles \
    . \
    ./GeneratedFiles/Debug \
    ./../../../include/

SOURCES += main.cpp\
        dialog.cpp

HEADERS  += dialog.h

FORMS    += dialog.ui

RESOURCES += iodrv_test.qrc

LIBS += \
        -L../../../lib/\
		-llog\
		-lfesapi


win32:RC_FILE = iodrv_test.rc