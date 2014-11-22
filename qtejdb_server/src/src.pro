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
    serverconfiguration.cpp \
    serverlistener.cpp \
    qejdbdatabaseservice.cpp \
    rcpsocketlistener.cpp

HEADERS += \
    server_global.h \
    serverconfiguration.h \
    serverlistener.h \
    qejdbdatabaseservice.h \
    rcpsocketlistener.h

include(../../3rdparty/qtargparser/QtArg/qtarg.pri)
#include(../../qtejdb/src/qtejdb.pri)

DESTDIR = ../../bin

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../qtejdb/src/release/ -lqtejdb
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../qtejdb/src/debug/ -lqtejdb
else:unix: LIBS += -L$$PWD/../../qtejdb/src/ -lqtejdb -lz

INCLUDEPATH += $$PWD/../../qtejdb/src
DEPENDPATH += $$PWD/../../qtejdb/src

# QtRpc2 lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../3rdparty/qtrcp2/build/lib/release/ -lqtrpc2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../3rdparty/qtrcp2/build/lib/debug/ -lqtrpc2
else:unix: LIBS += -L$$PWD/../../3rdparty/qtrcp2/build/lib/ -lqtrpc2

INCLUDEPATH += $$PWD/../../3rdparty/qtrcp2/include
INCLUDEPATH += $$PWD/../../3rdparty/qtrcp2/lib
DEPENDPATH += $$PWD/../../3rdparty/qtrcp2/lib
