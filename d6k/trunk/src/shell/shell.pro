# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = app
TARGET = shell
DESTDIR = ../../bin
QT += core gui
CONFIG += debug
DEFINES += _WINDOWS WIN64 QT_LARGEFILE_SUPPORT QT_DLL
INCLUDEPATH += . \
    ./GeneratedFiles \
    . \
    ./GeneratedFiles/Debug \
    ./GeneratedFiles/debug \
    $(QTDIR)/mkspecs/win32-msvc2013
LIBS += -lshell32
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
win32:RC_FILE = shell.rc
HEADERS += ./resource.h \
    ./shell.h
SOURCES += ./main.cpp \
    ./shell.cpp
