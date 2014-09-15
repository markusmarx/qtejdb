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

    QEjdbDatabase db = QEjdbDatabase::addDatabase(".", "test.db", QEJDB::CREATE | QEJDB::WRITE
                                          | QEJDB::LOCK_NB | QEJDB::TRUNCATE | QEJDB::LOCK_NB);
    bool open = db.open();
    QCOMPARE(open, true);

    QCOMPARE(db.isOpen(), true);

    QEjdbDatabase::removeDatabase();

}

void Tst_Database::tst_collection()
{
     QEjdbDatabase db = QEjdbDatabase::addDatabase(".", "test.db", QEJDB::CREATE | QEJDB::WRITE
                                           | QEJDB::LOCK_NB | QEJDB::TRUNCATE
                                           | QEJDB::LOCK_NB);

    if (!db.open()) {

    }

    bool exception = false;
    try {
        QEjdbCollection col = db.collection("testCollection");
        QCOMPARE(true, false);
    } catch(int error) {

    }

    QEjdbCollection col = db.createCollection("testCollection2");
    col = db.collection("testCollection2");

    QCOMPARE(QString("testCollection2"), col.collectionName());
    QCOMPARE(col.removeCollection(), true);
    try {
        QEjdbCollection col = db.collection("testCollection2");
        QCOMPARE(true, false);

    } catch(int error) {

    }



    QEjdbDatabase::removeDatabase();

}

void Tst_Database::cleanupTestCase()
{

    QEjdbDatabase::removeDatabaseFiles(".", "test.db");
}

