SOURCES += \
    ../3rdparty/ejdb/tcejdb/bson.c \
    ../3rdparty/ejdb/tcejdb/ejdb.c \
    ../3rdparty/ejdb/tcejdb/ejdbutl.c \
    ../3rdparty/ejdb/tcejdb/encoding.c \
    ../3rdparty/ejdb/tcejdb/md5.c \
    ../3rdparty/ejdb/tcejdb/myconf.c \
    ../3rdparty/ejdb/tcejdb/numbers.c \
    ../3rdparty/ejdb/tcejdb/nxjson.c \
    ../3rdparty/ejdb/tcejdb/platform.c \
    ../3rdparty/ejdb/tcejdb/tcadb.c \
    ../3rdparty/ejdb/tcejdb/tcbdb.c \
    ../3rdparty/ejdb/tcejdb/tcfdb.c \
    ../3rdparty/ejdb/tcejdb/tchdb.c \
    ../3rdparty/ejdb/tcejdb/tctdb.c \
    ../3rdparty/ejdb/tcejdb/tcutil.c \
    ../3rdparty/ejdb/tcejdb/utf8proc.c \
    ../3rdparty/ejdb/tcejdb/utf8proc_data.c \

HEADERS += \
    ../3rdparty/ejdb/tcejdb/bson.h \
    ../3rdparty/ejdb/tcejdb/ejdb.h \
    ../3rdparty/ejdb/tcejdb/ejdb_private.h \
    ../3rdparty/ejdb/tcejdb/ejdbutl.h \
    ../3rdparty/ejdb/tcejdb/encoding.h \
    ../3rdparty/ejdb/tcejdb/md5.h \
    ../3rdparty/ejdb/tcejdb/myconf.h \
    ../3rdparty/ejdb/tcejdb/nxjson.h \
    ../3rdparty/ejdb/tcejdb/tcadb.h \
    ../3rdparty/ejdb/tcejdb/tcbdb.h \
    ../3rdparty/ejdb/tcejdb/tcfdb.h \
    ../3rdparty/ejdb/tcejdb/tchdb.h \
    ../3rdparty/ejdb/tcejdb/tctdb.h \
    ../3rdparty/ejdb/tcejdb/tcutil.h \
    ../3rdparty/ejdb/tcejdb/utf8proc.h \


win32:SOURCES += ../3rdparty/ejdb/tcejdb/win32/platform.c
win32:HEADERS += ../3rdparty/ejdb/tcejdb/win32/platform.h

unix:SOURCES += ../3rdparty/ejdb/tcejdb/nix/platform.c
unix:HEADERS += ../3rdparty/ejdb/tcejdb/nix/platform.h
