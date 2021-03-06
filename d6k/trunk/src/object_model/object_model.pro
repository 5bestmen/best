# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = lib
TARGET = object_model

DESTDIR = ../../bin
QT += core  gui xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += debug
CONFIG += c++11

DEFINES += WIN64 QT_DLL QT_WIDGETS_LIB MODEL_TOOL_DLL
INCLUDEPATH += ./GeneratedFiles \
    . \
    ./GeneratedFiles/Debug \
    ./../../include/


DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles

win32:RC_FILE = equipmentmodel.rc
HEADERS += ./contentmodel.h \
    ./modeldefine.h \
    ./modelinfo.h \
    ./equipmentmodel.h \
    ./treeview.h \
    ./delegate.h \
    ./resource.h
SOURCES += ./contentmodel.cpp \
    ./delegate.cpp \
    ./equipmentmodel.cpp \
    ./main.cpp \
    ./modelinfo.cpp \
    ./treeview.cpp
RESOURCES += equipmentmodel.qrc
