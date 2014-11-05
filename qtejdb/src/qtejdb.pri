SOURCES += \
    $$PWD/qejdbdatabase.cpp \
    $$PWD/qejdbcollection.cpp \
    $$PWD/qejdbquery.cpp \
    $$PWD/bson/qbsonobject.cpp \
    $$PWD/bson/qbsonvalue.cpp \
    $$PWD/bson/qbsonarray.cpp \
    $$PWD/bson/qbsonoid.cpp \


HEADERS += \
    $$PWD/qejdbdatabase.h \
    $$PWD/qejdbcollection.h \
    $$PWD/qejdbquery.h \
    $$PWD/bson/qbsonobject.h \
    $$PWD/bson/qbsonvalue.h \
    $$PWD/bson/qbsonarray.h \
    $$PWD/bson/qbsonoid.h \
    $$PWD/bson/qbsonobject_p.h \

include($$PWD/../../3rdparty/ejdb/tcejdb/ejdb.pri)

INCLUDEPATH += $$PWD/../../3rdparty/ejdb/tcejdb

QMAKE_CFLAGS += -std=gnu99

LIBS += -lz
