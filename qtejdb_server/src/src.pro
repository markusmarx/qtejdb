#-------------------------------------------------
#
# Project created by QtCreator 2014-11-04T19:26:17
#
#-------------------------------------------------

QT       -= gui

TARGET = src
TEMPLATE = lib

DEFINES += SRC_LIBRARY

SOURCES +=

HEADERS += \
    server_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
