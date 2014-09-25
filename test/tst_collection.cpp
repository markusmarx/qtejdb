#include "tst_collection.h"
#include <QTest>
#include <QDebug>
#include "qejdbdatabase.h"
#include "bson/qbsonobject.h"
#include "bson/qbsonarray.h"

Tst_Collection::Tst_Collection(QObject *parent) :
    QObject(parent)
{
}

void Tst_Collection::initTestCase()
{
    QEjdbDatabase::removeDatabaseFiles(".", "test_db");
    QEjdbDatabase::addDatabase(".", "test_db", QEJDB::CREATE | QEJDB::WRITE
                             | QEJDB::LOCK_NB | QEJDB::TRUNCATE
                             | QEJDB::LOCK_NB).open();
}

void Tst_Collection::tst_simpleCRUD()
{
    QEjdbDatabase m_db = QEjdbDatabase::database();
    QEjdbCollection col = m_db.createCollection("testcollection");
    QBsonObject obj = createTestObject();
    obj.insert("test", QBsonValue("test"));

    col.save(obj);

    QBsonObject obj2 = col.load(obj.value("_id").toId().toString());
    QCOMPARE(obj.value("_id").toId(), obj2.value("_id").toId());

    QBsonObject obj3;
    obj3.insert("test", QBsonValue("test"));


    QBsonObject obj4;
    obj4.insert("test", "test");

    obj3.insert("inline", obj4);
    //qDebug() << obj3;

    col.save(obj3);

    QBsonObject obj5 = col.load(obj3.value("_id").toString());
    //qDebug() << obj5;

    QCOMPARE(obj3.value("_id").toString(), obj5.value("_id").toString());
    QCOMPARE(true, obj5.value("inline").isObject());
    QCOMPARE(obj3.value("inline").toObject().value("test"), obj5.value("inline").toObject().value("test"));

    //qDebug() << obj5;

    QCOMPARE(col.remove(obj5), true);

    //obj5 = col.load(obj3.value("_id").toString());

}

void Tst_Collection::tst_simpleQuery()
{
    QEjdbDatabase db = QEjdbDatabase::database();

    QList<QBsonObject> list = db.query("testcollection",
                                       QEjdbCondition("test",
                                                      QEjdbCondition::BEGIN, "tes"));
    QCOMPARE(list.size(), 1);

    list = db.query("testcollection",
                                       QEjdbCondition("test",
                                                      QEjdbCondition::EQUALS, "tes"));
    QCOMPARE(list.size(), 0);

    QVariantList qq;
    qq << "test" << "tes";

    list = db.query("testcollection", QEjdbCondition("test",
                                                     QEjdbCondition::IN, qq));

    QCOMPARE(list.size(), 1);

    qq.clear();
    qq << "te" << "tes";

    list = db.query("testcollection", QEjdbCondition("test",
                                                     QEjdbCondition::IN, qq));

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
    QEjdbCollection col = m_db.createCollection("datatypes");
    QBsonObject testObj = createTestObject();

    col.save(testObj);

    QBsonObject loaded = col.load(testObj.value("_id").toString());

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
}
