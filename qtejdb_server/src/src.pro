#-------------------------------------------------
#
# Project created by QtCreator 2014-11-04T19:26:17
#
#-------------------------------------------------
QT       += websockets
QT       -= gui

TARGET = qtejdbsrv
TEMPLATE = app

CONFIG += app_bundle

SOURCES += \
    main.cpp \
    echoserver.cpp

HEADERS += \
    server_global.h \
    echoserver.h

include(../../qtejdb/src/qtejdb.pri)

DESTDIR = ../../bin
