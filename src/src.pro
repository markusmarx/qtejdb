#-------------------------------------------------
#
# Project created by QtCreator 2014-09-05T15:44:05
#
#-------------------------------------------------

QT       += core
QT       -= gui

TARGET = qtejdb
TEMPLATE = lib
CONFIG += dll

SOURCES += \
    qejdbdatabase.cpp \
    qejdbcollection.cpp \
    qejdbquery.cpp \
    bson/qbsonobject.cpp \
    bson/qbsonvalue.cpp \
    bson/qbsonarray.cpp \
    bson/qbsonoid.cpp \


HEADERS += \
    qejdbdatabase.h \
    qejdbcollection.h \
    qejdbquery.h \
    bson/qbsonobject.h \
    bson/qbsonvalue.h \
    bson/qbsonarray.h \
    bson/qbsonoid.h \
    bson/qbsonobject_p.h \

include(ejdb.pri)


INCLUDEPATH += $$PWD/../3rdparty/ejdb/tcejdb

QMAKE_CFLAGS += -std=gnu99

OTHER_FILES += \
    ejdb.pri
