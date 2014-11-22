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
    tst_performance.cpp \
    tst_qbson.cpp \
    tst_worker.cpp

HEADERS += \
    tst_database.h \
    tst_collection.h \
    tst_performance.h \
    tst_qbson.h \
    tst_worker.h


# QtRpc2 lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../3rdparty/qtrcp2/build/lib/release/ -lqtrpc2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../3rdparty/qtrcp2/build/lib/debug/ -lqtrpc2
else:unix: LIBS += -L$$PWD/../../3rdparty/qtrcp2/build/lib/ -lqtrpc2

INCLUDEPATH += $$PWD/../../3rdparty/qtrcp2/include
INCLUDEPATH += $$PWD/../../3rdparty/qtrcp2/lib
DEPENDPATH += $$PWD/../../3rdparty/qtrcp2/build/lib

# QEjdb lib
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../src/release/ -lqtejdb
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../src/debug/ -lqtejdb
else:unix: LIBS += -L$$PWD/../src/ -lqtejdb -lz

INCLUDEPATH += $$PWD/../../3rdparty/ejdb/tcejdb
INCLUDEPATH += $$PWD/../src
DEPENDPATH += $$PWD/../src

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../src/release/libqtejdb.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../src/debug/libqtejdb.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../src/release/qtejdb.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../src/debug/qtejdb.lib
else:unix: PRE_TARGETDEPS += $$PWD/../src/libqtejdb.a


