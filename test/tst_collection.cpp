#include "tst_collection.h"
#include <QTest>
#include <QDebug>
#include "qejdbdatabase.h"
#include "bson/qbsonobject.h"

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
    QBsonObject obj;
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

    QList<QBsonObject> list = db.query("testcollection", QEjdbCondition("test", QEjdbCondition::BEGIN, "tes"));
    QCOMPARE(list.size(), 1);
    //qDebug() << list;
}

void Tst_Collection::tst_dataTypes()
{
    QEjdbDatabase m_db = QEjdbDatabase::database();
    QEjdbCollection col = m_db.createCollection("datatypes");

    QBsonObject testObj;
    testObj.insert("string", "test");
    testObj.insert("int", (int)10);
    testObj.insert("float", 11.334);
    testObj.insert("double", (double)11.3342233812344);
    testObj.insert("long", (long)2233113123123123223);
    testObj.insert("date", QDateTime(QDate(2014, 12, 12)));
    testObj.insert("timestamp", QDateTime());
    QByteArray ba;
    ba.append(12).append(23).append(47);

    testObj.insert("binary", QBsonValue(ba.data()));

    col.save(testObj);

    //qDebug() << testObj;
}

void Tst_Collection::cleanupTestCase()
{

    QEjdbDatabase::removeDatabase();
    QEjdbDatabase::removeDatabaseFiles(".", "test_db");
}
