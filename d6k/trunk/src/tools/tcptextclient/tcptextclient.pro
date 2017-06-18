#-------------------------------------------------
#
# Project created by QtCreator 2016-06-13T09:42:04
#
#-------------------------------------------------

QT       += core gui
QT += network
DESTDIR = ./bin

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tcptextclient
TEMPLATE = app


SOURCES += main.cpp\
        tcpclient.cpp

HEADERS  += tcpclient.h

FORMS    += tcpclient.ui
