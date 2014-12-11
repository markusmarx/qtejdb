#include "tst_qejdbcollection.h"
#include <QTest>
#include <QDebug>
#include "qejdbdatabase.h"
#include "qejdbquery.h"
#include "qbson/qbsonobject.h"
#include "qbson/qbsonarray.h"


Tst_QEjdbCollection::Tst_QEjdbCollection(QString url, QObject *parent) :
    QObject(parent), m_url(url)
{
}

/**
 * 
 */
void Tst_QEjdbCollection::initTestCase()
{
    QEjdbDatabase::removeDatabaseFiles(".", "test_db");
    QEjdbDatabase::addDatabase(m_url).open();
}

void Tst_QEjdbCollection::tst_simpleCRUD()
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
    QCOMPARE(2, m_db.loadAll("testcollection").count());

    QCOMPARE(m_db.remove("testcollection", obj5), true);
    obj5 = m_db.load("testcollection", obj5.value("_id").toString());
    QVERIFY(obj5.isEmpty());

    QCOMPARE(1, m_db.loadAll("testcollection").count());

}

void Tst_QEjdbCollection::tst_simpleQuery()
{
    QEjdbDatabase db = QEjdbDatabase::database();


    for (int i = 0; i < 1000; i++) {
        //qDebug() << i;

       QEjdbQuery query("testcollection", db);
       QBsonObject q = QBsonObject().append(
                           "test", QBsonObject("$begin", "tes"));

       QEjdbResult result = query.exec(q);

       QCOMPARE(result.count(), 1);

    }

    QEjdbQuery query("testcollection", db);
    QEjdbResult result = query.exec(QBsonObject("test", "tes"));
    QCOMPARE(result.count(), 0);


    result = query.exec(QBsonObject(
                          "test",
                            QBsonObject(
                              "$in", QBsonArray().append("test").append("tes")
                            )
                          )
                      );

    QCOMPARE(result.count(), 1);

    result = query.exec(QBsonObject(
                          "test",
                          QBsonObject(
                              "$in", QBsonArray().append("te").append("tes"))
                          )
                      );


    QCOMPARE(result.count(), 0);


}

QBsonObject Tst_QEjdbCollection::createTestObject()
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

void Tst_QEjdbCollection::tst_dataTypes()
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

/**
 * Test joins single and array values.
 *
 * @brief Tst_QEjdbCollection::tst_joins
 */
void Tst_QEjdbCollection::tst_joins()
{
    QEjdbDatabase m_db = QEjdbDatabase::database();
    m_db.createCollection("cars");
    m_db.createCollection("orders");

    // create 3 cars
    QBsonObject car1;
    car1.append("model", "Honda Accord");
    car1.append("year", 2005);
    m_db.save("cars", car1);

    QBsonObject car2;
    car2.append("model", "Toyota Corolla");
    car2.append("year", 2011);
    m_db.save("cars", car2);

    QBsonObject car3;
    car3.append("model", "Toyota Camry");
    car3.append("year", 2008);
    m_db.save("cars", car3);

    // create 2 orders
    // first with single car
    QBsonObject order1;
    order1
        .append("car", car1.value("_id"))
        .append("customer", "indy")
        .append("pickupDate", QBsonValue(QDateTime(QDate(2013, 11, 5))));
    m_db.save("orders", order1);

    // second with car array on same attibute
    QBsonObject order2;
    order2
        .append("car",
                QBsonArray()
                .append(car1.value("_id"))
                .append(car2.value("_id"))
                .append(car3.value("_id"))
        )
        .append("customer", "indy")
        .append("pickupDate", QBsonValue(QDateTime(QDate(2013, 11, 5))));

    m_db.save("orders", order2);

    // load first order and check car id
    order1 = m_db.load("orders", order1.value("_id").toString());
    QCOMPARE(car1.value("_id").toString(), order1.value("car").toString());

    // do a join an car attribute
    QBsonObject query("$do", QBsonObject(
                          "car", QBsonObject(
                              "$join", "cars")
                          )
                      );

    QEjdbResult result = m_db.query("orders", query);

    // check result
    QCOMPARE(2, result.count());

    // check single car on first order
    QCOMPARE(QString("Honda Accord"),
             result.first().value("car").toObject().value("model").toString());

    // check car array on second order
    QBsonArray carArray = result.values().at(1).value("car").toArray();
    QCOMPARE(3, carArray.size());
    QCOMPARE(QString("Honda Accord"),
             carArray.value(0).toObject().value("model").toString());
    QCOMPARE(QString("Toyota Corolla"),
             carArray.value(1).toObject().value("model").toString());
    QCOMPARE(QString("Toyota Camry"),
             carArray.value(2).toObject().value("model").toString());

    QByteArray b;
    QDataStream in(&b, QIODevice::WriteOnly);
    in << result;

    QEjdbResult resultOutStream;
    QDataStream out(&b, QIODevice::ReadOnly);
    out >> resultOutStream;

    QCOMPARE(resultOutStream.values().count(), 2);

}



void Tst_QEjdbCollection::cleanupTestCase()
{

    QEjdbDatabase::removeDatabase();
    QEjdbDatabase::removeDatabaseFiles(".", "test_db");

    //delete this;
}

