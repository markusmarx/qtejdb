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
TEST(QEjdbDatabaseTest, TestContainsCollection)
{
    QEjdbDatabase db = QEjdbDatabase::addDatabase(DBURL, "testconnection");
    EXPECT_FALSE(db.containsCollection(COLL));
    QEjdbDatabase::removeDatabase("testconnection");
}
TEST(QEjdbDatabaseTest, TestLoadBson)
{
    QEjdbDatabase db = QEjdbDatabase::addDatabase(DBURL);
    db.createCollection(COLL);
    QBsonObject obj, obj2;
    obj.append("name", 11);
    db.save(COLL, obj);
    obj2 = db.load(COLL, obj.oid());
    EXPECT_EQ(obj.oid(), obj2.oid());
    QEjdbDatabase::removeDatabase();
}
TEST(QEjdbDatabaseTest, TestRemoveBsonByOid)
{
    QEjdbDatabase db = QEjdbDatabase::addDatabase(DBURL);
    db.createCollection(COLL);
    QBsonObject obj;
    obj.append("name", 11);
    db.save(COLL, obj);
    db.remove(COLL, obj.oid());
    EXPECT_TRUE(db.load(COLL, obj.oid()).isEmpty());
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
    db.createCollection(COLL);
    QBsonObject obj;
    obj.append("name", 11);
    db.save(COLL, obj);
    QEjdbResult result = db.query(COLL, QBsonObject("name", 11));
    EXPECT_EQ(1, result.count());
    EXPECT_EQ(11, result.first().value("name"));
}
