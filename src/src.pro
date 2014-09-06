#-------------------------------------------------
#
# Project created by QtCreator 2014-09-05T15:44:05
#
#-------------------------------------------------

QT       += core
QT       -= gui

TARGET = qtejdb
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    qejdbdatabase.cpp \
    qejdbcollection.cpp

HEADERS += \
    qejdbdatabase.h \
    qejdbcollection.h


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../cpp.extern/ejdb/tcejdb/release/ -ltcejdb
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../cpp.extern/ejdb/tcejdb/debug/ -ltcejdb
else:unix: LIBS += -L$$PWD/../../../cpp.extern/ejdb/tcejdb/ -ltcejdb

INCLUDEPATH += $$PWD/../../../cpp.extern/ejdb/tcejdb
DEPENDPATH += $$PWD/../../../cpp.extern/ejdb/tcejdb

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../cpp.extern/ejdb/tcejdb/release/libtcejdb.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../cpp.extern/ejdb/tcejdb/debug/libtcejdb.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../cpp.extern/ejdb/tcejdb/release/tcejdb.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../cpp.extern/ejdb/tcejdb/debug/tcejdb.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../cpp.extern/ejdb/tcejdb/libtcejdb.a
