#include "tst_collection.h"
#include <QTest>
#include <QDebug>
#include "qejdbdatabase.h"

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

void Tst_Collection::tst_simpleCreate()
{
    QEjdbDatabase m_db = QEjdbDatabase::database();
    QEjdbCollection col = m_db.createCollection("testcollection");
    QJsonObject obj;
    obj.insert("test", QJsonValue("test"));

    col.save(obj);

    QJsonObject obj2 = col.load(obj.value("_id").toString());

    QCOMPARE(obj.value("_id"), obj2.value("_id"));

    QJsonObject obj3;
    obj3.insert("test", QJsonValue("test"));


    QJsonObject obj4;
    obj4.insert("test", "test");

    obj3.insert("inline", obj4);
    qDebug() << obj3;

    col.save(obj3);

    QJsonObject obj5 = col.load(obj3.value("_id").toString());
    qDebug() << obj5;
    QCOMPARE(obj3.value("_id"), obj5.value("_id"));
    QCOMPARE(true, obj5.value("inline").isObject());
    QCOMPARE(obj3.value("inline").toObject().value("test"), obj5.value("inline").toObject().value("test"));

    qDebug() << obj5;

}

void Tst_Collection::tst_simpleQuery()
{
    QEjdbDatabase db = QEjdbDatabase::database();

    QList<QJsonObject> list = db.query("testcollection", QEjdbCondition("test", QEjdbCondition::BEGIN, "tes"));
    QCOMPARE(list.size(), 2);
    qDebug() << list;
}

void Tst_Collection::tst_dataTypes()
{
    QEjdbDatabase m_db = QEjdbDatabase::database();
    QEjdbCollection col = m_db.createCollection("datatypes");

    QJsonObject testObj;
    testObj.insert("string", "test");
    testObj.insert("int", (int)10);
    testObj.insert("float", 11.334);
    testObj.insert("double", (double)11.3342233812344);
    testObj.insert("long", (qint64)2233113123123123223);
    testObj.insert("date", QDate(2014, 12, 12).toJulianDay());
    testObj.insert("timestamp", QDateTime().toMSecsSinceEpoch());
    QByteArray ba;
    ba.append(12).append(23).append(47);

    testObj.insert("binary", QJsonValue(ba.data()));

    col.save(testObj);

    qDebug() << testObj;
}

void Tst_Collection::cleanupTestCase()
{

    QEjdbDatabase::removeDatabase();
    QEjdbDatabase::removeDatabaseFiles(".", "test_db");
}
