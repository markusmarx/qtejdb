#include "tst_base.h"
#include "qbsonobject.h"
#include "qbsonarray.h"
#include <QTest>
Tst_Base::Tst_Base()
{

}

Tst_Base::~Tst_Base()
{

}

QBsonObject Tst_Base::createTestBsonObject(bool withId)
{
    QBsonObject subObj;
    QBsonObject testObj;
    if (withId) testObj.setOid(QBsonOid::generate());

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


    return testObj;
}



QList<QBsonObject> Tst_Base::createTestBsonObjectList(int count, bool withId)
{
    QList<QBsonObject> list;

    for (int i = 0; i < count; i++) {
        list.append(createTestBsonObject(withId));
    }
    return list;
}

void Tst_Base::testBsonObject(QBsonObject testObj)
{
    QCOMPARE(QString("test"), testObj.value("string").toString());
    QCOMPARE(10, testObj.value("int").toInt());
    QCOMPARE(11.334, testObj.value("float").toDouble());
    QCOMPARE(11.3342233812344, testObj.value("double").toDouble());
    QCOMPARE(2233113123123123223, testObj.value("long").toLong());
    QCOMPARE(QDateTime(QDate(2014, 12, 12)), testObj.value("date").toDateTime());
    QBsonObject loadedSubObj = testObj.value("subobj").toObject();
    QCOMPARE(true, testObj.value("subobj").isObject());
    QCOMPARE(QString("subinsert"), loadedSubObj.value("name").toString());
    QBsonArray loadedArr = testObj.value("array").toArray();
    QCOMPARE(true, testObj.value("array").isArray());
    QCOMPARE(3, loadedArr.size());
    QCOMPARE(QString("test"), loadedArr.value(0).toString());
    QCOMPARE(2222, loadedArr.value(1).toInt());
    loadedSubObj = loadedArr.value(2).toObject();
    QCOMPARE(true, loadedArr.value(2).isObject());
    QCOMPARE(QString("subinsert"), loadedSubObj.value("name").toString());
}
