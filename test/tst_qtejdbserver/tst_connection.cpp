#include "tst_connection.h"
#include "qejdbdatabase.h"
#include "qbson/qbsonobject.h"
#include "qbsonarray.h"
#include <QTest>

Tst_Connection::Tst_Connection(QObject *parent) :
    QObject(parent)
{
}

void Tst_Connection::tst_simple()
{

    QEjdbDatabase db = QEjdbDatabase::addDatabase(
                "socket:qtejdbtest", 0);

    db.open();
    QVERIFY(db.isOpen());

    QVERIFY(!db.containsCollection("testCollection"));
    QVERIFY(db.createCollection("testCollection"));
    QVERIFY(db.containsCollection("testCollection"));

    QBsonObject bsonObj;
    QBsonObject loadedBsonObj;
    for (int i = 0; i < 1000; i++) {
        bsonObj = createTestBson();

        QVERIFY(db.save("testCollection", bsonObj));
        QVERIFY(bsonObj.contains("_id"));
        QVERIFY(bsonObj.value("_id").toId().isValid());

        loadedBsonObj = db.load("testCollection", bsonObj.value("_id").toString());
        QCOMPARE(loadedBsonObj.value("_id").toString(), loadedBsonObj.value("_id").toString());

    }

    QEjdbResult resultList = db.query("testCollection", QBsonObject("test", QBsonObject("$begin", "tes")));
    QCOMPARE(resultList.count(), 1000);
    QCOMPARE(resultList.values().last().value("_id").toString(), loadedBsonObj.value("_id").toString());

    QVERIFY(db.remove("testCollection", bsonObj.value("_id").toString()));
    loadedBsonObj = db.load("testCollection", bsonObj.value("_id").toString());
    QVERIFY(loadedBsonObj.isEmpty());

    QVERIFY(db.removeCollection("testCollection"));
    QVERIFY(!db.containsCollection("testCollection"));
}

QBsonObject Tst_Connection::createTestBson()
{

    QBsonObject subObj;
    subObj.insert("subname", "test");
    QBsonArray arr;
    arr.append("test");
    arr.append(2222);
    arr.append(subObj);
    QBsonObject testObj;
    testObj.insert("test", "test");
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

QTEST_MAIN(Tst_Connection)
