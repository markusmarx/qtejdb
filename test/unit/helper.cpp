#include "helper.h"


QBsonObject createBsonObject(bool withId, int marker)
{
    QBsonObject subObj;
    QBsonObject testObj;
    if (withId) {
        testObj.setOid(QBsonOid::generate());
    }

    subObj.insert("name", "subinsert");
    QBsonArray arr;
    arr.append("test");
    arr.append(2222);
    arr.append(subObj);

    testObj.insert("string", "test");
    testObj.insert("int", (int)10);
    testObj.insert("float", 11.334);
    testObj.insert("double", (double)11.3342233812344);
    testObj.insert("long", (long)2233113123123123223);
    testObj.insert("date", QDateTime(QDate(2014, 12, 12)));
    testObj.insert("timestamp", QDateTime());
    testObj.insert("subobj", subObj);
    testObj.insert("array", arr);
    QByteArray ba;
    ba.append(12).append(23).append(47);

    testObj.insert("binary", QBsonValue(ba.data()));
    testObj.insert("marker", marker);
    return testObj;
}


QList<QBsonObject> createBsonObjects(uint count)
{
    QList<QBsonObject> list;
    for(int i = 0; i < count; i++) {
        list << createBsonObject(true, i);
    }
    return list;
}
