#include "tst_qbson.h"
#include "qbson/qbsonobject.h"
#include "qbson/qbsonoid.h"
#include "qbson/qbsonarray.h"
#include <QString>
#include <QTest>
#include <QElapsedTimer>
#include <QDebug>

void Tst_QBson::tst_qbson()
{
    QBsonObject b1;
    b1.insert("name1", QBsonValue(QLatin1String("test1")));

    QElapsedTimer  timer;
    timer.start();

    QDateTime d;
    char t[] = {12,12,12,23,23,23};
    QBsonObject b2;
    QBsonArray bsonArray;
    bsonArray.append(QBsonValue(1))
            .append(QBsonValue("name")).insert(0, QBsonValue(d));


    for (int i = 0; i < 1000; i++) {

        b2.insert("name1", QBsonValue(QLatin1String("test1")));
        b2.insert("name2", QBsonValue(10));
        b2.insert("name3", QBsonValue(10.1111));
        b2.insert("name4", QBsonValue(b1));
        b2.insert("name5", QBsonValue(d));
        b2.insert("name6", QBsonValue(QByteArray(t)));
        b2.insert("name7", QBsonValue(true));

        b2.insert("name8", QBsonValue(QBsonOid()));
        b2.insert("name9", QBsonValue(bsonArray));
    }

    QBsonObject b3 = b2;
    QCOMPARE(b3.value("name1").toString(), QString("test1"));
    QCOMPARE(b3.value("name2").toInt(), 10);
    QCOMPARE(b3.value("name3").toDouble(), 10.1111);
    QCOMPARE(b3.value("name4").toObject().
             value("name1").toString(),
             QString("test1"));
    QCOMPARE(b3.value("name5").toDateTime(), d);
    QCOMPARE(b3.value("name6").toBinary(), QByteArray(t));
    QCOMPARE(b3.value("name7").toBool(), true);
    QCOMPARE(b3.value("name8").toId(), b2.value("name8").toId());
    QCOMPARE(b3.value("name9").toArray().size(), bsonArray.size());
    QCOMPARE(b3.value("name9").toArray().value(0).toDateTime(), d);
    QCOMPARE(b3.value("name9").toArray().value(1).toInt(), 1);
    QCOMPARE(b3.value("name9").toArray().value(2).toString(), QString("name"));
    QCOMPARE(
                QBsonValue::fromVariant(b3.value("name1").toVariant()).toString()
                , QString("test1") );
    QCOMPARE(
                QBsonValue::fromVariant(b3.value("name2").toVariant()).toInt()
                , 10 );
    QCOMPARE(
                QBsonValue::fromVariant(b3.value("name3").toVariant()).toDouble()
                , 10.1111 );
    QCOMPARE(
                QBsonValue::fromVariant(
                    b3.value("name4").toVariant()).toObject()
                        .value("name1").toString()
                , QString("test1") );
    QCOMPARE(
                QBsonValue::fromVariant(b3.value("name5").toVariant()).toDateTime()
                , d );
    QCOMPARE(
                QBsonValue::fromVariant(b3.value("name6").toVariant()).toBinary()
                , QByteArray(t));
    QCOMPARE(
                QBsonValue::fromVariant(b3.value("name7").toVariant()).toBool()
                , true );
    QCOMPARE(
                QBsonValue::fromVariant(b3.value("name8").toVariant()).toId()
                ,  b2.value("name8").toId());
    QCOMPARE(
                QBsonValue::fromVariant(b3.value("name9").toVariant()).toArray().size()
                ,  3);
}

/**
 * @brief Tst_QBson::tst_failover test fails with ranges or values that dont
 * exist.
 */
void Tst_QBson::tst_failover()
{
    QBsonObject obj;

    obj.value("name").toString();
    obj.value("name").toArray().value(1);
    obj.value("name").toObject().value("name");


}

void Tst_QBson::tst_stream()
{
    QDateTime d = QDateTime::currentDateTime();
    const char t[] = {12,12,12,23,23,23};
    QBsonObject b1;
    b1.insert("name1", QBsonValue(QLatin1String("test1")));

    QBsonObject b2;
    QByteArray ba(t);
    b2.insert("name1", QBsonValue(QLatin1String("test1")));
    b2.insert("name2", QBsonValue(10));
    b2.insert("name3", QBsonValue(10.1111));
    b2.insert("name4", QBsonValue(b1));
    b2.insert("name5", QBsonValue(d));
    b2.insert("name6", QBsonValue(ba));
    b2.insert("name7", QBsonValue(true));
    b2.insert("name8", QBsonValue(QBsonOid()));

    QByteArray stream = b2.toBinary();

    QBsonObject b3(stream);
     //bson_print_raw(b3.toBinary().constData(), 1);
    QCOMPARE(b3.value("name1").toString(), QString("test1"));
    QCOMPARE(b3.value("name2").toInt(), 10);
    QCOMPARE(b3.value("name3").toDouble(), 10.1111);
    QCOMPARE(b3.value("name4").toObject().
             value("name1").toString(),
             QString("test1"));
    QCOMPARE(b3.value("name5").toDateTime(), d);
    QCOMPARE(b3.value("name6").toBinary().size(), ba.size());
    QCOMPARE(b3.value("name7").toBool(), true);
    QCOMPARE(b3.value("name8").toId().toString(), b2.value("name8").toId().toString());

    for (int i = 0; i < 10000; i++) {
        QBsonObject bs;
        bs.insert("name", QBsonValue("test"));
        stream = bs.toBinary();
        QBsonObject bs1(stream);
    }

    for (int i = 0; i < 10000; i++) {
        QBsonObject bs = b3;
        stream = bs.toBinary();
        QBsonObject bs1(stream);
        stream = bs.toBinary();
    }

    QByteArray streamArray;
    QDataStream dw(&streamArray, QIODevice::WriteOnly);
    dw << b3;
    QDataStream dr(&streamArray, QIODevice::ReadOnly);
    QBsonObject b4;
    dr >> b4;

    QCOMPARE(b4.values().size(), b3.values().size());
    QCOMPARE(b4.value("name1").toString(), QString("test1"));
    QCOMPARE(b4.value("name2").toInt(), 10);
    QCOMPARE(b4.value("name3").toDouble(), 10.1111);
    QCOMPARE(b4.value("name4").toObject().
             value("name1").toString(),
             QString("test1"));
    QCOMPARE(b4.value("name5").toDateTime(), d);
    QCOMPARE(b4.value("name6").toBinary().size(), ba.size());
    QCOMPARE(b4.value("name7").toBool(), true);
    QCOMPARE(b4.value("name8").toId().toString(), b2.value("name8").toId().toString());

    QList<QBsonObject> lw;
    lw.append(b4);
    streamArray.clear();
    QList<QBsonObject> lr;
    QDataStream dwL(&streamArray, QIODevice::WriteOnly);
    dwL << lw;

    QDataStream drL(&streamArray, QIODevice::ReadOnly);
    drL >> lr;

    QCOMPARE(1, lr.size());
    b4 = lr.first();
    QCOMPARE(b4.values().size(), b3.values().size());
    QCOMPARE(b4.value("name1").toString(), QString("test1"));
    QCOMPARE(b4.value("name2").toInt(), 10);
    QCOMPARE(b4.value("name3").toDouble(), 10.1111);
    QCOMPARE(b4.value("name4").toObject().
             value("name1").toString(),
             QString("test1"));
    QCOMPARE(b4.value("name5").toDateTime(), d);
    QCOMPARE(b4.value("name6").toBinary().size(), ba.size());
    QCOMPARE(b4.value("name7").toBool(), true);
    QCOMPARE(b4.value("name8").toId().toString(), b2.value("name8").toId().toString());

}

void Tst_QBson::cleanupTestCase()
{
    //delete this;
}

void Tst_QBson::tst_list()
{
    QDateTime d = QDateTime::currentDateTime();
    char t[] = {12,12,12,23,23,23};
    QList<QBsonObject> list;
    for (int i = 0; i < 1000; ++i) {
        QBsonObject b1;
        b1.insert("name1", QBsonValue(QString("test")));

        QBsonObject b2;
        b2.insert("name1", QBsonValue(QString("test %1").arg(i)));
        b2.insert("name2", QBsonValue(10));
        b2.insert("name3", QBsonValue(10.1111));
        b2.insert("name4", QBsonValue(b1));
        b2.insert("name5", QBsonValue(d));
        b2.insert("name6", QBsonValue(QByteArray(t)));
        b2.insert("name7", QBsonValue(true));
        b2.insert("name8", QBsonValue(QBsonOid()));
        list.append(b2);
    }

    for(int i = 0; i < list.size(); ++i) {
        QBsonObject v = list.at(i);
        QCOMPARE(v.value("name1").toString(), QString("test %1").arg(i));
    }

    while (list.size() > 0) list.removeAt(0);



}

QTEST_MAIN(Tst_QBson)
