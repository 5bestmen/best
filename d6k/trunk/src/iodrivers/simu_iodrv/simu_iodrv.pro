#-------------------------------------------------
#
# Project created by QtCreator 2016-05-11T16:32:06
#
#-------------------------------------------------

QT       += network xml core gui 
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = simu_iodrv
TEMPLATE = lib

CONFIG += debug
CONFIG += c++11

DESTDIR = ../../../bin

DEFINES += IO_DRIVER WIN64

INCLUDEPATH += ./GeneratedFiles \
    . \
    ./GeneratedFiles/Debug \
     $(QTDIR)/mkspecs/win32-msvc2013 \
    ./../../../include/  
    
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
win32:RC_FILE = simu_iodrv.rc

SOURCES += iodrv_main.cpp \
           channel.cpp \
		   device.cpp \
		   tag.cpp \
		   dll_lib.cpp \
           simu_task.cpp \
           simu_channel.cpp \
		   simu_device.cpp \   
           simu_point.cpp \ 
           tcpclient.cpp

HEADERS += simu_task.h \
           channel.h \
		   device.h \
		   tag.h \
		   dll_lib.h \
          simu_channel.h \ 
		   simu_device.h \           
           ../../../include/log/log.h \     
           ../../../include/fesapi/iodriver_main.h \
           ../../../include/fesapi/fesdatadef.h \
           ../../../include/fesapi/fesapi.h \
            simu_point.h \ 
            tcpclient.h

LIBS += \
        -L../../../lib/\
		-llog\
		-lfesapi \

unix {
    target.path = /usr/lib
    INSTALLS += target
}
