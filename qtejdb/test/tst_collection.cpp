#include "tst_collection.h"
#include <QTest>
#include <QDebug>
#include "qejdbdatabase.h"
#include "qejdbquery.h"
#include "bson/qbsonobject.h"
#include "bson/qbsonarray.h"


Tst_Collection::Tst_Collection(QObject *parent) :
    QObject(parent)
{
}

void Tst_Collection::initTestCase()
{
    QEjdbDatabase::removeDatabaseFiles(".", "test_db");
    QEjdbDatabase::addDatabase("file:test_db", QEJDB::CREATE | QEJDB::WRITE
                             | QEJDB::LOCK_NB | QEJDB::TRUNCATE).open();
}

void Tst_Collection::tst_simpleCRUD()
{
    QEjdbDatabase m_db = QEjdbDatabase::database();
    m_db.createCollection("testcollection");
    QBsonObject obj = createTestObject();

    obj.insert("test", QBsonValue("test"));

    m_db.save("testcollection", obj);

    QBsonObject obj2 = m_db.load("testcollection", obj.value("_id").toString());
    QCOMPARE(obj.value("_id").toId(), obj2.value("_id").toId());

    QBsonObject obj3;
    obj3.insert("test", QBsonValue("test"));


    QBsonObject obj4;
    obj4.insert("test", "test");

    obj3.insert("inline", obj4);
    //qDebug() << obj3;

    m_db.save("testcollection", obj3);

    QBsonObject obj5 = m_db.load("testcollection", obj3.value("_id").toString());
    //qDebug() << obj5;

    QCOMPARE(obj3.value("_id").toString(), obj5.value("_id").toString());
    QCOMPARE(true, obj5.value("inline").isObject());
    QCOMPARE(obj3.value("inline").toObject().value("test"), obj5.value("inline").toObject().value("test"));

    //qDebug() << obj5;

    QCOMPARE(m_db.remove("testcollection", obj5), true);

    obj5 = m_db.load("testcollection", obj5.value("_id").toString());
    QVERIFY(obj5.isEmpty());

}

void Tst_Collection::tst_simpleQuery()
{
    QEjdbDatabase db = QEjdbDatabase::database();


    for (int i = 0; i < 1000; i++) {
        //qDebug() << i;
       QList<QBsonObject> list;
       QEjdbQuery query("testcollection", db);
       QBsonObject q = QBsonObject().append(
                           "test", QBsonObject("$begin", "tes"));
       list = query.exec(q);

       QCOMPARE(list.size(), 1);

    }

    QEjdbQuery query("testcollection", db);
    QList<QBsonObject> list = query.exec(QBsonObject("test", "tes"));
    QCOMPARE(list.size(), 0);


    list = query.exec(QBsonObject(
                          "test",
                            QBsonObject(
                              "$in", QBsonArray().append("test").append("tes")
                            )
                          )
                      );

    QCOMPARE(list.size(), 1);



    list = query.exec(QBsonObject(
                          "test",
                          QBsonObject(
                              "$in", QBsonArray().append("te").append("tes"))
                          )
                      );


    QCOMPARE(list.size(), 0);


}

QBsonObject Tst_Collection::createTestObject()
{
    QBsonObject subObj;
    subObj.insert("name", "subinsert");
    QBsonArray arr;
    arr.append("test");
    arr.append(2222);
    arr.append(subObj);
    QBsonObject testObj;
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

void Tst_Collection::tst_dataTypes()
{
    QEjdbDatabase m_db = QEjdbDatabase::database();
    m_db.createCollection("datatypes");
    QBsonObject testObj = createTestObject();

    m_db.save("datatypes", testObj);

    QBsonObject loaded = m_db.load("datatypes", testObj.value("_id").toString());

    QCOMPARE(QString("test"), loaded.value("string").toString());
    QCOMPARE(10, loaded.value("int").toInt());
    QCOMPARE(11.334, loaded.value("float").toDouble());
    QCOMPARE(11.3342233812344, loaded.value("double").toDouble());
    QCOMPARE(2233113123123123223, loaded.value("long").toLong());
    QCOMPARE(QDateTime(QDate(2014, 12, 12)), loaded.value("date").toDateTime());
    QBsonObject loadedSubObj = loaded.value("subobj").toObject();
    QCOMPARE(true, loaded.value("subobj").isObject());
    QCOMPARE(QString("subinsert"), loadedSubObj.value("name").toString());
    QBsonArray loadedArr = loaded.value("array").toArray();
    QCOMPARE(true, loaded.value("array").isArray());
    QCOMPARE(3, loadedArr.size());
    QCOMPARE(QString("test"), loadedArr.value(0).toString());
    QCOMPARE(2222, loadedArr.value(1).toInt());
    loadedSubObj = loadedArr.value(2).toObject();
    QCOMPARE(true, loadedArr.value(2).isObject());
    QCOMPARE(QString("subinsert"), loadedSubObj.value("name").toString());
    //qDebug() << testObj;
}



void Tst_Collection::cleanupTestCase()
{

    QEjdbDatabase::removeDatabase();
    QEjdbDatabase::removeDatabaseFiles(".", "test_db");

    delete this;
}
