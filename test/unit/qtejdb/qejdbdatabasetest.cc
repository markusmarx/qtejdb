#include "gtest/gtest.h"
#include "qtejdb/qejdbdatabase.h"
#include <QFile>

#define MODE QEJDB::CREATE | QEJDB::WRITE | \
    QEJDB::SYNC | QEJDB::LOCK_NB | QEJDB::TRUNCATE

#define DBURL "file:test_database.db"
#define COLL "testCollection"

TEST(QEjdbDatabaseTest, TestAddDatabaseWithUrl)
{
    QEjdbDatabase db = QEjdbDatabase::addDatabase(DBURL);
    EXPECT_TRUE(db.isOpen());
    QEjdbDatabase::removeDatabase();
    EXPECT_TRUE(QFile("test_database.db").exists());
}
TEST(QEjdbDatabaseTest, TestRemoveDatabaseFiles)
{
    QEjdbDatabase db = QEjdbDatabase::addDatabase(DBURL);
    db.createCollection(COLL);
    db.close();
    QEjdbDatabase::removeDatabaseFiles("", "test_database.db");
    EXPECT_FALSE(QFile("test_database.db").exists());
}
TEST(QEjdbDatabaseTest, TestConnectionName)
{
    QEjdbDatabase db = QEjdbDatabase::addDatabase(DBURL, "testconnection");
    EXPECT_EQ("testconnection", db.connectionName());
    QEjdbDatabase::removeDatabase("testconnection");
}
TEST(QEjdbDatabaseTest, TestCollectionWithNonExist)
{
    QEjdbDatabase db = QEjdbDatabase::addDatabase(DBURL, "testconnection");
    EXPECT_FALSE(db.collection(QLatin1String(COLL)).exist());
    QEjdbDatabase::removeDatabase("testconnection");
}
TEST(QEjdbDatabaseTest, TestCollectionWithCreateAndDrop)
{
    QEjdbDatabase db = QEjdbDatabase::addDatabase(DBURL, "testconnection");
    EXPECT_TRUE(db.collection(QLatin1String(COLL), true).exist());
    db.collection(COLL).drop();
    EXPECT_FALSE(db.collection(QLatin1String(COLL)).exist());
    QEjdbDatabase::removeDatabase("testconnection");
}
TEST(QEjdbDatabaseTest, TestCollectionWithKilledDb)
{
    QEjdbDatabase db = QEjdbDatabase::addDatabase(DBURL, "testconnection");
    QEjdbCollection col = db.collection(QLatin1String(COLL));
    QEjdbDatabase::removeDatabase("testconnection");
    EXPECT_FALSE(col.exist());
    EXPECT_FALSE(col.create());
    EXPECT_FALSE(col.drop());
}
TEST(QEjdbDatabaseTest, TestLoadBson)
{
    QEjdbDatabase db = QEjdbDatabase::addDatabase(DBURL);
    QEjdbCollection col = db.collection(COLL, true);
    QBsonObject obj, obj2;
    obj.append("name", 11);
    col.save(obj);
    obj2 = col.load(obj.oid());
    EXPECT_EQ(obj.oid(), obj2.oid());
    col.drop();
    QEjdbDatabase::removeDatabase();
}
TEST(QEjdbDatabaseTest, TestRemoveBsonByOid)
{
    QEjdbDatabase db = QEjdbDatabase::addDatabase(DBURL);
    QEjdbCollection col = db.collection(COLL, true);
    QBsonObject obj;
    obj.append("name", 11);
    db.save(COLL, obj);
    col.remove(obj.oid());
    EXPECT_TRUE(col.load(obj.oid()).isEmpty());
    QEjdbDatabase::removeDatabase();
}
TEST(QEjdbDatabaseTest, TestRemoveBsonByObject)
{
    QEjdbDatabase db = QEjdbDatabase::addDatabase(DBURL);
    db.createCollection(COLL);
    QBsonObject obj;
    obj.append("name", 11);
    db.save(COLL, obj);
    db.remove(COLL, obj);
    EXPECT_TRUE(db.load(COLL, obj.oid()).isEmpty());
    QEjdbDatabase::removeDatabase();
}
TEST(QEjdbDatabaseTest, TestQuery)
{
    QEjdbDatabase db = QEjdbDatabase::addDatabase(DBURL, MODE);
    QEjdbCollection col = db.collection(COLL, true);
    QBsonObject obj;
    obj.append("name", 11);
    col.save(obj);
    QEjdbResult result = col.query(QBsonObject("name", 11));
    EXPECT_EQ(1, result.count());
    EXPECT_EQ(11, result.first().value("name"));
    QEjdbDatabase::removeDatabase();
}
