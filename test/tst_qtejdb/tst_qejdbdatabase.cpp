#include "tst_qejdbdatabase.h"
#include <QTest>
#include <QDebug>
#include "qejdbworker.h"
#include "qejdbrpcworker.h"

Tst_QEjdbDatabase::Tst_QEjdbDatabase(QObject *parent) :
    QObject(parent)
{
}

/**
 * @brief Tst_QEjdbDatabase::tst_open tests open a database.
 */
void Tst_QEjdbDatabase::tst_open()
{

    QEjdbDatabase db = QEjdbDatabase::addDatabase("file:test.db");
    m_db2 = db;

    try {
        db.open();
    } catch (QEjdbException &ex) {
        Q_ASSERT_X(false, Q_FUNC_INFO, ex.message().toLatin1());
    }

    QCOMPARE(db.isOpen(), true);

    QEjdbDatabase::removeDatabase();

}

/**
 * @brief tst_openWithDir Open the database in a subdirctory and try to open a
 * database twice.
 */
void Tst_QEjdbDatabase::tst_openWithDir()
{

    QDir testDir;
    testDir.mkdir("testdir");

    QEjdbDatabase db = QEjdbDatabase::addDatabase(
                "file:testdir/test.db",
                QEJDB::CREATE | QEJDB::WRITE | QEJDB::LOCK_NB
                | QEJDB::TRUNCATE | QEJDB::LOCK_NB);
    db.open();
    db.open();

    QCOMPARE(db.isOpen(), true);

    QEjdbDatabase db2 = QEjdbDatabase::addDatabase(
                "file:testdir/test.db",
                QEJDB::CREATE | QEJDB::WRITE | QEJDB::LOCK_NB
                | QEJDB::TRUNCATE | QEJDB::LOCK_NB,
                QStringLiteral("connection2"));
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

void Tst_QEjdbDatabase::tst_collection()
{
    QEjdbDatabase db = QEjdbDatabase::addDatabase("file:test.db");
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

/**
 * @brief Tst_QEjdbDatabase::tst_workers test to create workers by uri.
 */
void Tst_QEjdbDatabase::tst_workers()
{

    //
    // create file worker.
    //

    QEjdbWorker *worker = QEjdbWorker::createFromUrl(QUrl("file:test"), 0);

    Q_ASSERT(worker);
    Q_ASSERT(dynamic_cast<QEjdbFileWorker*>(worker));
    QVERIFY(dynamic_cast<QEjdbFileWorker*>(worker)->mode() != 0);
    //
    // create  rpc worker
    //
    worker = QEjdbWorker::createFromUrl(QUrl("tcp://localhost:9090"), 0);
    Q_ASSERT(worker);
    Q_ASSERT(dynamic_cast<QEjdbRpcWorker*>(worker));

}

void Tst_QEjdbDatabase::cleanupTestCase()
{

    QEjdbDatabase::removeDatabaseFiles(".", "test.db");
    //delete this;
}

QTEST_MAIN(Tst_QEjdbDatabase)
