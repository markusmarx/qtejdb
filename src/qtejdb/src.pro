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

QMAKE_CFLAGS += -std=gnu99

SOURCES += \
    qejdbdatabase.cpp \
    qejdbcollection.cpp \
    qejdbquery.cpp \
    bson/qbsonobject.cpp \
    bson/qbsonvalue.cpp \
    bson/qbsonarray.cpp \
    bson/qbsonoid.cpp \
    qejdbworker.cpp \
    qejdbfileworker.cpp \
    qejdbrpcworker.cpp \
    qejdbexception.cpp \
    qejdbresult.cpp


HEADERS += \
    qejdbdatabase.h \
    qejdbcollection.h \
    qejdbquery.h \
    bson/qbsonobject.h \
    bson/qbsonvalue.h \
    bson/qbsonarray.h \
    bson/qbsonoid.h \
    bson/qbsonobject_p.h \
    qejdbworker.h \
    qejdbrpcworker.h \
    qejdbexception.h \
    qejdbresult.h \
    qejdbresult_p.h

INCLUDEPATH += ../../3rdparty/ejdb/tcejdb
include(../../3rdparty/ejdb/tcejdb/ejdb.pri)

# QtRpc2 lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../3rdparty/qtrcp2/build/lib/release/ -lqtrpc2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../3rdparty/qtrcp2/build/lib/debug/ -lqtrpc2
else:unix: LIBS += -L$$PWD/../../3rdparty/qtrcp2/build/lib/ -lqtrpc2

INCLUDEPATH += $$PWD/../../3rdparty/qtrcp2/include
INCLUDEPATH += $$PWD/../../3rdparty/qtrcp2/lib
DEPENDPATH += $$PWD/../../3rdparty/qtrcp2/build/lib

OTHER_FILES += \
    qtejdb.pri
