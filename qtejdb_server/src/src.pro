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
    echoserver.cpp \
    serverconfiguration.cpp \
    serverlistener.cpp \
    websocketlistener.cpp

HEADERS += \
    server_global.h \
    echoserver.h \
    serverconfiguration.h \
    serverlistener.h \
    websocketlistener.h

include(../../3rdparty/qtargparser/QtArg/qtarg.pri)
include(../../qtejdb/src/qtejdb.pri)

DESTDIR = ../../bin

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../qtejdb/src/release/ -lqtejdb
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../qtejdb/src/debug/ -lqtejdb
#else:unix: LIBS += $$PWD/../../qtejdb/src/libqtejdb.a -lz

INCLUDEPATH += $$PWD/../../qtejdb/src
#DEPENDPATH += $$PWD/../../qtejdb/src
