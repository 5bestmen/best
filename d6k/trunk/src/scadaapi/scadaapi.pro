#-------------------------------------------------
#
# Project created by QtCreator 2016-04-18T09:46:35
#
#-------------------------------------------------


TEMPLATE = lib
TARGET = scadaapi
DESTDIR = ../../bin

QT += core  gui

DEFINES += SCADAAPI_LIBRARY WIN64  DEBUG SCADA_SDK_LIB

CONFIG += debug

INCLUDEPATH += . \
    ./GeneratedFiles \
    ./GeneratedFiles/Debug \
    ./../../include \
    ./../../thirdparty/ACE_wrappers \    
    ./GeneratedFiles/debug \
    $(QTDIR)/mkspecs/win32-msvc2013

LIBS += \
        -L../../lib/\
		-llog \
		-lmail \
    	-L../../thirdparty/ACE_wrappers/lib/\
        -lACEd

DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles

win32:RC_FILE = scadaapi.rc

TRANSLATIONS += scadaapi-zh-CN.ts
    
RESOURCES  += scadaapi.qrc


SOURCES += scadaapi.cpp \
           tagname_pool.cpp \
           scadasvc_api.cpp \
           sharemem.cpp \
           base_mem_db.cpp \
           base_module.cpp \
           fes_db.cpp \
           mem_db.cpp \
           db_svc.cpp \
           ../scdpub/variant_data.h \

HEADERS += scadaapi.h\
        scadaapi_global.h  \
        ../../include/scadaapi/scdsvcapi.h \
        scdsvcapi.h \
        sharemem.h \
        tagname_pool.h \
        ./base_mem_db.h \
         base_module.h \
         fes_db.h \
         mem_db.h \
         db_svc.h \
         ../scdpub/variant_data.cpp \

