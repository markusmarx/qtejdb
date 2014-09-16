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
    qejdbcondition.cpp \
    qbsonobject.cpp

HEADERS += \
    qejdbdatabase.h \
    qejdbcollection.h \
    qejdbquery.h \
    qejdbcondition.h \
    globals_p.h \
    qbsonobject.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../3rdparty/ejdb/tcejdb/release/ -ltcejdb
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../3rdparty/ejdb/tcejdb/debug/ -ltcejdb
else:unix: LIBS += $$PWD/../3rdparty/ejdb/tcejdb/libtcejdb.a

INCLUDEPATH += $$PWD/../3rdparty/ejdb/tcejdb
DEPENDPATH += $$PWD/../3rdparty/ejdb/tcejdb

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../3rdparty/ejdb/tcejdb/release/libtcejdb.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../3rdparty/ejdb/tcejdb/debug/libtcejdb.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../3rdparty/ejdb/tcejdb/release/tcejdb.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../3rdparty/ejdb/tcejdb/debug/tcejdb.lib
else:unix: PRE_TARGETDEPS += $$PWD/../3rdparty/ejdb/tcejdb/libtcejdb.a
