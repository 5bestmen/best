#-------------------------------------------------
#
# Project created by QtCreator 2015-12-29T09:39:05
#
#-------------------------------------------------

QT       += core gui
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = graph
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        graphview.cpp\
        graphscene.cpp\
        graph.cpp\
        graph_file.cpp\
        graph_folder.cpp\
        background.cpp\        
        base_widget.cpp\
        line_widget.cpp\
        shape_widget.cpp\
        group_widget.cpp\
        graphics_layer.cpp\
        spin_item.cpp\
        base_dync.cpp\
        dync_color.cpp\
        dync_value.cpp

HEADERS  += mainwindow.h\
           graphview.h\
           graphscene.h\
           graph.h\
           graph_file.h\
           background.h\        
           base_widget.h\
           line_widget.h\
           shape_widget.h\
           group_widget.h\
           graphics_layer.h\
           spin_item.h \
           colour_define.h \
           base_dync.h\
           dync_color.h\
           dync_value.h
           

FORMS    += mainwindow.ui

RC_ICONS = "painter.ico"
RESOURCES  += graph.qrc
