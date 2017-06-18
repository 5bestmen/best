#-------------------------------------------------
#
# Project created by QtCreator 2016-05-11T16:32:06
#
#-------------------------------------------------

QT       += network xml core gui 
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ppgf
TEMPLATE = lib

CONFIG += debug
CONFIG += c++11

DESTDIR = ../../../bin

DEFINES += SCADA_APP WIN64

INCLUDEPATH += ./GeneratedFiles \
    . \
    ./GeneratedFiles/Debug \
     $(QTDIR)/mkspecs/win32-msvc2015 \
    ./../../../include/  
    
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
win32:RC_FILE = ppgf.rc

SOURCES += app_main.cpp \
           dll_lib.cpp \
           base_module.cpp \
           dll_lib.cpp \
           ext_service.cpp \ 
           pv_station.cpp \
           inverter.cpp \
           tag.cpp \ 
  

HEADERS += app_main.h \             
           ../../../include/log/log.h \     
           ../../../include/scadaapi/scdapp_api.h \
           ../../../include/scadaapi/scdapp_def.h \ 
            dll_lib.h \
            ext_service.h \ 
            pv_station.h \
            inverter.h \
            tag.h \ 
            app_main.h \

LIBS += \
        -L../../../lib/\
		-llog\
		-lscadaapi \

unix {
    target.path = /usr/lib
    INSTALLS += target
}
