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

void Tst_Database::tst_openWithDir()
{

    QDir testDir;
    testDir.mkdir("testdir");

    QEjdbDatabase db = QEjdbDatabase::addDatabase("testdir", "test.db", QEJDB::CREATE | QEJDB::WRITE
                                          | QEJDB::LOCK_NB | QEJDB::TRUNCATE | QEJDB::LOCK_NB);
    bool open = db.open();
    QCOMPARE(open, true);

    QCOMPARE(db.isOpen(), true);

    QEjdbDatabase::removeDatabase();

    testDir.cd("testdir");
    QFile testDb(testDir.filePath("test.db"));
    QCOMPARE(testDb.exists(), true);

    QEjdbDatabase::removeDatabaseFiles("testdir", "test.db");


    testDir.cdUp();
    testDir.rmpath("testdir");
}

void Tst_Database::tst_collection()
{
     QEjdbDatabase db = QEjdbDatabase::addDatabase(".", "test.db", QEJDB::CREATE | QEJDB::WRITE
                                           | QEJDB::LOCK_NB | QEJDB::TRUNCATE
                                           | QEJDB::LOCK_NB);

    if (!db.open()) {

    }


    QEjdbCollection col = db.collection("testCollection");
    QCOMPARE(col.isValid(), false);

    col = db.createCollection("testCollection2");
    col = db.collection("testCollection2");

    QCOMPARE(QString("testCollection2"), col.collectionName());
    QCOMPARE(db.removeCollection("testCollection2"), true);

    QEjdbCollection col1 = db.collection("testCollection2");
    QCOMPARE(false, col1.isValid());

    QEjdbDatabase::removeDatabase();

}

void Tst_Database::cleanupTestCase()
{

    QEjdbDatabase::removeDatabaseFiles(".", "test.db");
    delete this;
}

