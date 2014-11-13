#-------------------------------------------------
#
# Project created by QtCreator 2014-11-13T10:31:15
#
#-------------------------------------------------

QT       += core testlib

QT       -= gui

TARGET = test
CONFIG   += console testcase
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    tst_connection.cpp \
    qejdbdatabaseservice.cpp

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

HEADERS += \
    tst_connection.h \
    qejdbdatabaseservice.h
