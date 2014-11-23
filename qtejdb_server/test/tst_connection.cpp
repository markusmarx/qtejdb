#include "tst_connection.h"
#include "qejdbdatabase.h"
#include "bson/qbsonobject.h"
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
    bsonObj.insert("test", QBsonValue("test"));

    QVERIFY(db.save("testCollection", bsonObj));
    QVERIFY(bsonObj.contains("_id"));
    QVERIFY(bsonObj.value("_id").toId().isValid());

    QBsonObject loadedBsonObj = db.load("testCollection", bsonObj.value("_id").toString());
    QCOMPARE(loadedBsonObj.value("_id").toString(), loadedBsonObj.value("_id").toString());

    QEjdbResult resultList = db.query("testCollection", QBsonObject("test", QBsonObject("$begin", "tes")));
    QCOMPARE(resultList.count(), 1);
    QCOMPARE(resultList.first().value("_id").toString(), loadedBsonObj.value("_id").toString());

    QVERIFY(db.remove("testCollection", bsonObj.value("_id").toString()));
    loadedBsonObj = db.load("testCollection", bsonObj.value("_id").toString());
    QVERIFY(loadedBsonObj.isEmpty());

    QVERIFY(db.removeCollection("testCollection"));
    QVERIFY(!db.containsCollection("testCollection"));
}
