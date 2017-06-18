#-------------------------------------------------
#
# Project created by QtCreator 2016-05-11T16:32:06
#
#-------------------------------------------------

QT       += network xml core gui 
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mbsim
TEMPLATE = lib

DESTDIR = ../../../bin

DEFINES += DS_LIBRARY WIN64

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
win32:RC_FILE = mbsim.rc

SOURCES += mbsim_module.cpp \
           mbsim.cpp \
           mbnet.cpp \
           memdata.cpp \
           modbus.cpp \
		   coilview.cpp

HEADERS += mbsim_module.h \
        ds_global.h \
        asio_base.h \
        asio_buffer.h \
        asio_client.h \
        asio_connector.h \
        asio_object.h \
        asio_packer.h \
        asio_socket.h \
        asio_tcp_client.h \
        asio_tcp_socket.h \
        asio_timer.h \
        asio_unpacker.h \
        object_pool.h \
        service_pump.h \
        task.h \     
        iosvc_timer.h\           
         ../../../include/devicestudio/plugin_module.h \
        mbsim.h\
        mbnet.h\
        mbtcp_def.h\
        mb_unpacker.h \
        memdata.h \
        modbus.h \
		coilview.h
FORMS += ./ui/coildlg.ui    
unix {
    target.path = /usr/lib
    INSTALLS += target
}
