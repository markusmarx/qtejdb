#include "tst_database.h"
#include "qejdbdatabase.h"
#include <QTest>
#include <QDebug>

Tst_Database::Tst_Database(QObject *parent) :
    QObject(parent)
{
}

void Tst_Database::tst_open()
{

    QEJDBDatabase *db = new QEJDBDatabase();
    bool open = db->open(".", "test.db", QEJDB::CREATE | QEJDB::WRITE
                     | QEJDB::LOCK_NB | QEJDB::TRUNCATE
                     | QEJDB::LOCK_NB);
    QCOMPARE(open, true);

    QCOMPARE(db->isOpen(), true);

    db->close();

}

void Tst_Database::tst_collection()
{
     QEJDBDatabase *db = new QEJDBDatabase();

    if (!db->open(".", "test.db", QEJDB::CREATE | QEJDB::WRITE
                  | QEJDB::LOCK_NB | QEJDB::TRUNCATE
                  | QEJDB::LOCK_NB)) {

    }

    bool exception = false;
    try {
        QEJDBCollection col = db->collection("testCollection");
        qDebug() << &col;

    } catch(int error) {
        exception = true;
    }

    QCOMPARE(exception, true);

    QEJDBCollection col = db->createCollection("testCollection2");
    col = db->collection("testCollection2");

    QCOMPARE(QString("testCollection2"), col.collectionName());

    db->close();

}

void Tst_Database::cleanupTestCase()
{
    QEJDBDatabase *db = new QEJDBDatabase();

    if (!db->open(".", "test.db", QEJDB::CREATE | QEJDB::WRITE
                  | QEJDB::LOCK_NB | QEJDB::TRUNCATE
                  | QEJDB::LOCK_NB)) {

    }

    db->remove();
}

