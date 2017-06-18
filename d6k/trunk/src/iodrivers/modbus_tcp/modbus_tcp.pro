#-------------------------------------------------
#
# Project created by QtCreator 2016-05-11T16:32:06
#
#-------------------------------------------------

QT       += network xml core gui 
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = modbus_tcp
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
win32:RC_FILE = modbus_tcp.rc

SOURCES += iodrv_main.cpp \
           channel.cpp \
		   device.cpp \
		   tag.cpp \
		   dll_lib.cpp \
           modbus_task.cpp \
           modbus_channel.cpp \
		   modbus_device.cpp \   
           modbus_point.cpp \ 
           tcpclient.cpp

HEADERS += modbus_task.h \
           channel.h \
		   device.h \
		   tag.h \
		   dll_lib.h \
           modbus_channel.h \ 
		   modbus_device.h \           
           ../../../include/log/log.h \     
           ../../../include/fesapi/iodriver_main.h \
           ../../../include/fesapi/fesdatadef.h \
           ../../../include/fesapi/fesapi.h \
            modbus_point.h \ 
            tcpclient.h

LIBS += \
        -L../../../lib/\
		-llog\
		-lfesapi \

unix {
    target.path = /usr/lib
    INSTALLS += target
}
