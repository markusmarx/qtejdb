#include "gtest/gtest.h"
#include "qtejdb/qejdbworker.h"
#include "qtejdb/qejdbdatabase.h"
#include "qtejdb/qejdbexception.h"
#include <QFile>
#define MODE QEJDB::CREATE | QEJDB::WRITE | \
    QEJDB::SYNC | QEJDB::LOCK_NB | QEJDB::TRUNCATE

#define DBURL QUrl("file:test_fileworker.db")

TEST(QEjdbFileWorkerTest, TestOpen)
{
    QEjdbFileWorker *fw = new QEjdbFileWorker(DBURL, MODE);
    fw->open();
    EXPECT_TRUE(fw->isOpen());
    EXPECT_TRUE(QFile("test_fileworker.db").exists());
    EXPECT_EQ(MODE, fw->mode());
    fw->close();
    delete fw;
}
TEST(QEjdbFileWorkerTest, TestOpenTwice)
{
    QEjdbFileWorker *fw = new QEjdbFileWorker(DBURL, MODE);
    fw->open();
    fw->open();
    EXPECT_TRUE(fw->isOpen());
    EXPECT_TRUE(QFile("test_fileworker.db").exists());
    fw->close();
    delete fw;
}
TEST(QEjdbFileWorkerTest, TestOpenWithLock)
{
    QEjdbFileWorker *fw = new QEjdbFileWorker(DBURL, MODE);
    fw->open();
    QEjdbFileWorker *fw2 = new QEjdbFileWorker(DBURL, MODE);
    EXPECT_THROW(fw2->open(), QEjdbException);

    fw->close();
    delete fw;
    delete fw2;
}
TEST(QEjdbFileWorkerTest, TestCreateAndRemoveCollection)
{
    QEjdbFileWorker *fw = new QEjdbFileWorker(DBURL, MODE);
    fw->open();
    EXPECT_FALSE(fw->containsCollection("testCollection"));
    EXPECT_TRUE(fw->createCollection("testCollection"));
    EXPECT_TRUE(fw->containsCollection("testCollection"));
    EXPECT_TRUE(fw->removeCollection("testCollection"));
    EXPECT_FALSE(fw->containsCollection("testCollection"));
    fw->close();
    delete fw;
}
TEST(QEjdbFileWorkerTest, TestRemoveInvalidCollection)
{
    QEjdbFileWorker *fw = new QEjdbFileWorker(DBURL, MODE);
    fw->open();
    EXPECT_FALSE(fw->removeCollection("testCollection"));
    fw->close();
    delete fw;
}
TEST(QEjdbFileWorkerTest, TestSave)
{
    QEjdbFileWorker *fw = new QEjdbFileWorker(DBURL, MODE);
    fw->open();
    QBsonObject obj;
    EXPECT_TRUE(fw->createCollection("testCollection"));
    obj.append("test", 11);
    EXPECT_TRUE(fw->save("testCollection", obj));
    EXPECT_TRUE(obj.hasOid());
    EXPECT_TRUE(fw->removeCollection("testCollection"));
    fw->close();
    delete fw;
}
TEST(QEjdbFileWorkerTest, TestLoad)
{
    QEjdbFileWorker *fw = new QEjdbFileWorker(DBURL, MODE);
    fw->open();
    QBsonObject obj, obj2;
    EXPECT_TRUE(fw->createCollection("testCollection"));
    obj.append("test", 11);
    EXPECT_TRUE(fw->save("testCollection", obj));
    obj2 = fw->load("testCollection", obj.oid());
    EXPECT_EQ(obj.oid(), obj2.oid());
    EXPECT_EQ(11, obj2.value("test"));
    EXPECT_TRUE(fw->removeCollection("testCollection"));
    fw->close();
    delete fw;
}
TEST(QEjdbFileWorkerTest, TestRemove)
{
    QEjdbFileWorker *fw = new QEjdbFileWorker(DBURL, MODE);
    fw->open();
    QBsonObject obj;
    EXPECT_TRUE(fw->createCollection("testCollection"));
    obj.append("test", 11);
    EXPECT_TRUE(fw->save("testCollection", obj));
    EXPECT_TRUE(fw->remove("testCollection", obj.oid()));
    EXPECT_TRUE(fw->load("testCollection", obj.oid()).isEmpty());
    EXPECT_TRUE(fw->removeCollection("testCollection"));
    fw->close();
    delete fw;
}
TEST(QEjdbFileWorkerTest, TestQuery)
{
    QEjdbFileWorker *fw = new QEjdbFileWorker(DBURL, MODE);
    fw->open();
    QBsonObject obj;
    EXPECT_TRUE(fw->createCollection("testCollection"));
    obj.append("test", 11);
    EXPECT_TRUE(fw->save("testCollection", obj));
    QEjdbResult result = fw->query("testCollection", QBsonObject("test", 11));
    EXPECT_EQ(1, result.count());
    EXPECT_TRUE(fw->removeCollection("testCollection"));
    fw->close();
    delete fw;
}
TEST(QEjdbFileWorkerTest, TestLoadAll)
{
    QEjdbFileWorker *fw = new QEjdbFileWorker(DBURL, MODE);
    fw->open();
    QBsonObject obj, obj2;
    EXPECT_TRUE(fw->createCollection("testCollection"));
    obj.append("test", 11);
    obj2.append("test1", 12);
    EXPECT_TRUE(fw->save("testCollection", obj));
    EXPECT_TRUE(fw->save("testCollection", obj2));
    QEjdbResult result = fw->loadAll("testCollection");
    EXPECT_EQ(2, result.count());
    EXPECT_TRUE(fw->removeCollection("testCollection"));
    fw->close();
    delete fw;
}
