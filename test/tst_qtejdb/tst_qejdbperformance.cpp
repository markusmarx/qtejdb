#include "tst_qejdbperformance.h"
#include "qejdbdatabase.h"
#include "qbson/qbsonobject.h"
#include "qbson/qbsonarray.h"
#include <QTest>
#include <QElapsedTimer>
#include <QDebug>

Tst_QEjdbPerformance::Tst_QEjdbPerformance(QString url, QObject *parent) :
    QObject(parent), m_url(url)
{
}

void Tst_QEjdbPerformance::initTestCase()
{
    QEjdbDatabase::removeDatabaseFiles(".", "test_db");
    QEjdbDatabase::addDatabase(m_url).open();
}

void Tst_QEjdbPerformance::cleanupTestCase()
{

    QEjdbDatabase::removeDatabase();
    QEjdbDatabase::removeDatabaseFiles(".", "test_db");

}

void Tst_QEjdbPerformance::tst_insQryTsd()
{
    QEjdbDatabase m_db = QEjdbDatabase::database();
    m_db.removeCollection("testcollection");
    m_db.createCollection("testcollection");

    QString json = QString("{'name': 'test'\n 'num': '10'\n 'description': 'test test test test test test test'}");

    QElapsedTimer t;


    QBsonObject obj;
    obj.insert("name", QBsonValue("test"));
    obj.insert("name1", QBsonValue("test"));
    obj.insert("name2", QBsonValue("test"));
    obj.insert("name3", QBsonValue("test"));
    obj.insert("name4", QBsonValue("test"));
    obj.insert("name5", QBsonValue("test"));
    obj.insert("num", QBsonValue(10));
    obj.insert("num1", QBsonValue(10));
    obj.insert("num2", QBsonValue(10));
    obj.insert("num3", QBsonValue(10));
    obj.insert("num4", QBsonValue(10));
    obj.insert("num5", QBsonValue(10));
    obj.insert("description", QBsonValue("test test test test test test test test test test test test test test test test"));
    obj.insert("description1", QBsonValue("test test test test test test test test test test test test test test test test"));
    obj.insert("description2", QBsonValue("test test test test test test test test test test test test test test test test"));
    obj.insert("description3", QBsonValue("test test test test test test test test test test test test test test test test"));
    obj.insert("description4", QBsonValue("test test test test test test test test test test test test test test test test"));
    QBsonArray arr;
    arr.append("test1");
    arr.append("test2");
    arr.append("test3");
    arr.append("test4");
    obj.insert("array", arr);
    //qDebug() << obj;
    int iteration = 1000;
    t.start();

    QString id;
    for (int j = 0; j < 5; j++) {

        for (int i = 0; i < iteration; i++) {
            obj.remove("_id");
            m_db.save("testcollection", obj);
        }
        qDebug() << iteration << " inserts costs " << t.elapsed() << "ms per insert " << (double)t.elapsed()/iteration << "ms";
        t.restart();

        QEjdbResult r = m_db.loadAll("testcollection");
        QCOMPARE(r.values().count(), (j+1)*iteration);
        qDebug() << iteration << "load" << (j+1)*iteration << " documents costs " << t.elapsed() << "ms";
        t.restart();
    }



}
