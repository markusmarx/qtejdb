TEMPLATE = subdirs

SUBDIRS += src \
    test

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/3rdparty/ejdb/tcejdb/release/ -ltcejdb
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/3rdparty/ejdb/tcejdb/debug/ -ltcejdb
else:unix: LIBS += -L$$PWD/3rdparty/ejdb/tcejdb/ -ltcejdb

INCLUDEPATH += $$PWD/3rdparty/ejdb/tcejdb
DEPENDPATH += $$PWD/3rdparty/ejdb/tcejdb

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/3rdparty/ejdb/tcejdb/release/libtcejdb.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/3rdparty/ejdb/tcejdb/debug/libtcejdb.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/3rdparty/ejdb/tcejdb/release/tcejdb.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/3rdparty/ejdb/tcejdb/debug/tcejdb.lib
else:unix: PRE_TARGETDEPS += $$PWD/3rdparty/ejdb/tcejdb/libtcejdb.a
