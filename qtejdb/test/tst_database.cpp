#include "tst_database.h"
#include <QTest>
#include <QDebug>

Tst_Database::Tst_Database(QObject *parent) :
    QObject(parent)
{
}

void Tst_Database::tst_open()
{

    QEjdbDatabase db = QEjdbDatabase::addDatabase("file:test.db", QEJDB::CREATE | QEJDB::WRITE
                                          | QEJDB::LOCK_NB | QEJDB::TRUNCATE | QEJDB::LOCK_NB);
    m_db2 = db;

    try {
        db.open();
    } catch (QEjdbException &ex) {

    }

    QCOMPARE(db.isOpen(), true);

    QEjdbDatabase::removeDatabase();

}

void Tst_Database::tst_openWithDir()
{

    QDir testDir;
    testDir.mkdir("testdir");

    QEjdbDatabase db = QEjdbDatabase::addDatabase("file:testdir/test.db", QEJDB::CREATE | QEJDB::WRITE
                                          | QEJDB::LOCK_NB | QEJDB::TRUNCATE | QEJDB::LOCK_NB);
    db.open();
    db.open();

    QCOMPARE(db.isOpen(), true);

    QEjdbDatabase db2 = QEjdbDatabase::addDatabase("file:testdir/test.db", QEJDB::CREATE | QEJDB::WRITE
                                          | QEJDB::LOCK_NB | QEJDB::TRUNCATE | QEJDB::LOCK_NB, QStringLiteral("connection2"));
    try {
        db2.open();
        Q_ASSERT_X(false, Q_FUNC_INFO, "database should permitted to open");
    } catch (QEjdbException &ex) {
        // it works.
    }

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
    QEjdbDatabase db = QEjdbDatabase::addDatabase("file:test.db", QEJDB::CREATE | QEJDB::WRITE
                                           | QEJDB::LOCK_NB | QEJDB::TRUNCATE
                                           | QEJDB::LOCK_NB);
    //
    // test with closed database.
    //
    try {
        db.containsCollection("testCollection");
        Q_ASSERT_X(false, "tst_collection", "a exception is expected.");
    } catch (QEjdbException &ex) {
        // it works
    }

    try {
        db.createCollection("testCollection2");
        Q_ASSERT_X(false, "tst_collection", "a exception is expected.");
    } catch (QEjdbException &ex) {
        // it works
    }

    try {
        QCOMPARE(db.removeCollection("testCollection2"), true);
        Q_ASSERT_X(false, "tst_collection", "a exception is expected.");
    } catch (QEjdbException &ex) {
        // it works
    }

    //
    // test with opened database.
    //
    db.open();

    QVERIFY(!db.containsCollection("testCollection"));

    QVERIFY(db.createCollection("testCollection2"));

    QVERIFY(db.removeCollection("testCollection2"));

    QVERIFY(!db.containsCollection(QStringLiteral("testCollection2")));

    QEjdbDatabase::removeDatabase();

}

void Tst_Database::cleanupTestCase()
{

    QEjdbDatabase::removeDatabaseFiles(".", "test.db");
    delete this;
}

