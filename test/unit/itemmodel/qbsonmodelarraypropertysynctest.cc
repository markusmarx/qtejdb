#include "gtest/gtest.h"
#include "itemmodel/qejdbarraypropertysync.h"
#include "../helper.h"
#include <QDebug>

#define MODE QEJDB::CREATE | QEJDB::WRITE | \
    QEJDB::SYNC | QEJDB::LOCK_NB | QEJDB::TRUNCATE

#define DBURL "file:test_arraypropertysync.db"
#define COLL "testCollection"
#define COLL2 "testCollection2"
#define PROP "arrayProperty"


class QBsonModelArrayPropertySyncTest : public ::testing::Test
{
    protected:

        static void SetUpTestCase()
        {
            QEjdbDatabase::addDatabase(DBURL, MODE);
            QEjdbDatabase::database().createCollection(COLL);
            QEjdbDatabase::database().createCollection(COLL2);

            QBsonObject masterObject;
            QBsonObject masterObjectWithoutJoin(PROP, QBsonArray());

            QBsonArray array;
            for (int i = 1; i < 11; i++) {
                QBsonObject obj = createBsonObject(false, i);
                QEjdbDatabase::database().save(COLL, obj);
                array.append(obj.oid());
                masterObjectWithoutJoin.value(PROP).toArray().append(createBsonObject(false, i));
            }
            masterObject.append(PROP, array);
            QEjdbDatabase::database().save(COLL2, masterObject);
            QEjdbDatabase::database().save(COLL2, masterObjectWithoutJoin);
            QBsonModelArrayPropertySyncTest::object = masterObject;
            QBsonModelArrayPropertySyncTest::objectWithoutJoin = masterObjectWithoutJoin;
        }

        static void TearDownTestCase()
        {
            QEjdbDatabase::database().dropCollection(COLL);
            QEjdbDatabase::database().dropCollection(COLL2);
            QEjdbDatabase::removeDatabase();
        }
    public:
        static QBsonObject object, objectWithoutJoin;

};

QBsonObject QBsonModelArrayPropertySyncTest::object;
QBsonObject QBsonModelArrayPropertySyncTest::objectWithoutJoin;

TEST_F(QBsonModelArrayPropertySyncTest, TestFetchWithJoinedProperty)
{
    QEjdbArrayPropertySync *sync = new QEjdbArrayPropertySync(
                QEjdbDatabase::database());
    sync->setCollection(COLL2);
    sync->setPropertyCollection(COLL);
    sync->setBsonObject(QBsonModelArrayPropertySyncTest::object, PROP);

    sync->fetch();
    EXPECT_EQ(COLL2, sync->collection());
    EXPECT_EQ(COLL, sync->propertyCollection());
    EXPECT_EQ(10, sync->model()->count());
    EXPECT_EQ(1, sync->model()->row(0).value("marker"));
    EXPECT_EQ(QBsonModelArrayPropertySyncTest::object, sync->bsonObject());
    EXPECT_EQ(PROP, sync->propertyName());
    delete sync;
}
TEST_F(QBsonModelArrayPropertySyncTest, TestUpdateWithJoinedProperty)
{
    QEjdbArrayPropertySync *sync = new QEjdbArrayPropertySync(
                QEjdbDatabase::database());
    sync->setCollection(COLL2);
    sync->setPropertyCollection(COLL);
    sync->setBsonObject(QBsonModelArrayPropertySyncTest::object, PROP);
    sync->fetch();
    sync->model()->update("string", "neu", 0);
    sync->fetch();
    EXPECT_EQ("neu", sync->model()->row(0).value("string"));
    EXPECT_EQ(10, sync->model()->count());
    delete sync;
}
TEST_F(QBsonModelArrayPropertySyncTest, TestInsertWithJoinedProperty)
{
    QEjdbArrayPropertySync *sync = new QEjdbArrayPropertySync(
                QEjdbDatabase::database());
    sync->setCollection(COLL2);
    sync->setPropertyCollection(COLL);
    sync->setBsonObject(QBsonModelArrayPropertySyncTest::object, PROP);
    sync->fetch();
    QBsonObject obj = createBsonObject(false, 0);
    sync->model()->insert(obj, 0);
    sync->fetch();
    EXPECT_EQ(0, sync->model()->row(0).value("marker"));
    EXPECT_EQ(11, sync->model()->count());
    sync->model()->remove(0);
    delete sync;
}
TEST_F(QBsonModelArrayPropertySyncTest, TestRemoveWithJoinedProperty)
{
    QEjdbArrayPropertySync *sync = new QEjdbArrayPropertySync(
                QEjdbDatabase::database());
    sync->setCollection(COLL2);
    sync->setPropertyCollection(COLL);
    sync->setBsonObject(QBsonModelArrayPropertySyncTest::object, PROP);
    QBsonObject removedObject = sync->model()->row(0);
    sync->fetch();
    sync->model()->remove(0);
    EXPECT_EQ(2, sync->model()->row(0).value("marker"));
    EXPECT_EQ(9, sync->model()->count());
    sync->fetch();
    EXPECT_EQ(2, sync->model()->row(0).value("marker"));
    EXPECT_EQ(9, sync->model()->count());
    removedObject = QEjdbDatabase::database().load(COLL, removedObject.oid());
    EXPECT_TRUE(removedObject.isEmpty());
    sync->model()->insert(createBsonObject(false, 1), 0);
    delete sync;
}
TEST_F(QBsonModelArrayPropertySyncTest, TestMovedWithJoinedProperty)
{
    QEjdbArrayPropertySync *sync = new QEjdbArrayPropertySync(
                QEjdbDatabase::database());
    sync->setCollection(COLL2);
    sync->setPropertyCollection(COLL);
    sync->setBsonObject(QBsonModelArrayPropertySyncTest::object, PROP);
    sync->fetch();
    sync->model()->move(0, 1);
    EXPECT_EQ(2, sync->model()->row(0).value("marker"));
    EXPECT_EQ(1, sync->model()->row(1).value("marker"));
    EXPECT_EQ(10, sync->model()->count());
    sync->fetch();
    EXPECT_EQ(2, sync->model()->row(0).value("marker"));
    EXPECT_EQ(1, sync->model()->row(1).value("marker"));
    EXPECT_EQ(10, sync->model()->count());
    delete sync;
}
TEST_F(QBsonModelArrayPropertySyncTest, TestFetchWithNoJoinedProperty)
{
    QEjdbArrayPropertySync *sync = new QEjdbArrayPropertySync(
                QEjdbDatabase::database());
    sync->setCollection(COLL2);
    sync->setBsonObject(QBsonModelArrayPropertySyncTest::objectWithoutJoin, PROP);
    sync->fetch();
    EXPECT_EQ(COLL2, sync->collection());
    EXPECT_EQ(10, sync->model()->count());
    EXPECT_EQ(1, sync->model()->row(0).value("marker"));
    EXPECT_EQ(2, sync->model()->row(1).value("marker"));
    delete sync;
}
TEST_F(QBsonModelArrayPropertySyncTest, TestUpdateWithNoJoinedProperty)
{
    QEjdbArrayPropertySync *sync = new QEjdbArrayPropertySync(
                QEjdbDatabase::database());
    sync->setCollection(COLL2);
    sync->setBsonObject(QBsonModelArrayPropertySyncTest::objectWithoutJoin, PROP);
    sync->fetch();
    sync->model()->update("string", "neu", 0);
    sync->fetch();
    EXPECT_EQ("neu", sync->model()->row(0).value("string"));
    EXPECT_EQ(10, sync->model()->count());
    delete sync;
}
TEST_F(QBsonModelArrayPropertySyncTest, TestInsertWithNoJoinedProperty)
{
    QEjdbArrayPropertySync *sync = new QEjdbArrayPropertySync(
                QEjdbDatabase::database());
    sync->setCollection(COLL2);
    sync->setBsonObject(QBsonModelArrayPropertySyncTest::objectWithoutJoin, PROP);
    sync->fetch();
    QBsonObject obj = createBsonObject(false, 0);
    sync->model()->insert(obj, 0);
    sync->fetch();
    EXPECT_EQ(0, sync->model()->row(0).value("marker"));
    EXPECT_EQ(11, sync->model()->count());
    sync->model()->remove(0);
    delete sync;
}
TEST_F(QBsonModelArrayPropertySyncTest, TestRemoveWithNoJoinedProperty)
{
    QEjdbArrayPropertySync *sync = new QEjdbArrayPropertySync(
                QEjdbDatabase::database());
    sync->setCollection(COLL2);
    sync->setBsonObject(QBsonModelArrayPropertySyncTest::objectWithoutJoin, PROP);
    sync->fetch();
    sync->model()->remove(0);
    EXPECT_EQ(2, sync->model()->row(0).value("marker"));
    EXPECT_EQ(9, sync->model()->count());
    sync->fetch();
    EXPECT_EQ(2, sync->model()->row(0).value("marker"));
    EXPECT_EQ(9, sync->model()->count());
    sync->model()->insert(createBsonObject(false, 1), 0);
    delete sync;
}
TEST_F(QBsonModelArrayPropertySyncTest, TestMovedWithNoJoinedProperty)
{
    QEjdbArrayPropertySync *sync = new QEjdbArrayPropertySync(
                QEjdbDatabase::database());
    sync->setCollection(COLL2);
    sync->setBsonObject(QBsonModelArrayPropertySyncTest::objectWithoutJoin, PROP);
    sync->fetch();
    sync->model()->move(0, 1);
    EXPECT_EQ(2, sync->model()->row(0).value("marker"));
    EXPECT_EQ(1, sync->model()->row(1).value("marker"));
    EXPECT_EQ(10, sync->model()->count());
    sync->fetch();
    EXPECT_EQ(2, sync->model()->row(0).value("marker"));
    EXPECT_EQ(1, sync->model()->row(1).value("marker"));
    EXPECT_EQ(10, sync->model()->count());
    delete sync;
}
