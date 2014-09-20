#include "tst_performance.h"
#include "qejdbdatabase.h"
#include "bson/qbsonobject.h"
#include "bson/qbsonarray.h"
#include <QTest>
#include <QElapsedTimer>
#include <QDebug>

Tst_Performance::Tst_Performance(QObject *parent) :
    QObject(parent)
{
}

void Tst_Performance::initTestCase()
{
    QEjdbDatabase::removeDatabaseFiles(".", "test_db");
    QEjdbDatabase::addDatabase(".", "test_db", QEJDB::CREATE | QEJDB::WRITE
                             | QEJDB::LOCK_NB | QEJDB::TRUNCATE
                             | QEJDB::LOCK_NB | QEJDB::SYNC).open();
}

void Tst_Performance::cleanupTestCase()
{

    QEjdbDatabase::removeDatabase();
    QEjdbDatabase::removeDatabaseFiles(".", "test_db");
}

void Tst_Performance::tst_insQryTsd()
{
    QEjdbDatabase m_db = QEjdbDatabase::database();
    QEjdbCollection col = m_db.createCollection("testcollection");

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
            col.save(obj);
            if (i == 1) id = obj.value("_id").toString();
        }
        qDebug() << iteration << " inserts costs " << t.elapsed() << "ms per insert " << (double)t.elapsed()/iteration << "ms";
        t.restart();
    }

   /* QList<QJsonObject> result = m_db.query("testcollection", QEjdbCondition("name", QEjdbCondition::EQUALS, "test"));
    qDebug() << "found " << result.size() << " data";
    qDebug() << result.at(0);
    qDebug() << col.load(id);*/

}
