#-------------------------------------------------
#
# Project created by QtCreator 2014-09-05T16:07:55
#
#-------------------------------------------------

QT       += core testlib

QT       -= gui

TARGET = test
CONFIG   += console testcase
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    tst_database.cpp \
    tst_collection.cpp \
    tst_performance.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../src/release/ -lqtejdb
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../src/debug/ -lqtejdb
else:unix: LIBS += -L$$OUT_PWD/../src/ -lqtejdb

INCLUDEPATH += $$PWD/../src
INCLUDEPATH += $$PWD/../../../cpp.extern/ejdb/tcejdb
DEPENDPATH += $$PWD/../src

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../src/release/libqtejdb.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../src/debug/libqtejdb.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../src/release/qtejdb.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../src/debug/qtejdb.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../src/libqtejdb.a

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


HEADERS += \
    tst_database.h \
    tst_collection.h \
    tst_performance.h
