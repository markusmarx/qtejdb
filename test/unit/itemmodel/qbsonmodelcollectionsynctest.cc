#include "gtest/gtest.h"
#include "itemmodel/qbsonitemmodelsync_p.h"
#include "../helper.h"
#include <QDebug>

#define MODE QEJDB::CREATE | QEJDB::WRITE | \
    QEJDB::SYNC | QEJDB::LOCK_NB | QEJDB::TRUNCATE

#define DBURL "file:test_collectionsync.db"
#define COLL "testCollection"

class QBsonModelCollectionSyncTest : public ::testing::Test
{
    protected:

        static void SetUpTestCase()
        {
            QEjdbDatabase::addDatabase(DBURL, MODE);
            QEjdbDatabase::database().createCollection(COLL);

            for (int i = 1; i < 11; i++) {
                QBsonObject obj = createBsonObject(false, i);
                QEjdbDatabase::database().save(COLL, obj);
            }
        }

        static void TearDownTestCase()
        {
            QEjdbDatabase::database().removeCollection(COLL);
            QEjdbDatabase::removeDatabase();
        }

};


TEST_F(QBsonModelCollectionSyncTest, TestFetch)
{
    QEjdbCollectionSync *sync = new QEjdbCollectionSync(
                QEjdbDatabase::database());
    sync->setCollection(COLL);

    sync->fetch();
    EXPECT_EQ(COLL, sync->collection());
    EXPECT_EQ(10, sync->model()->count());
    delete sync;
}
TEST_F(QBsonModelCollectionSyncTest, TestItemUpdate)
{
    QEjdbCollectionSync *sync = new QEjdbCollectionSync(
                QEjdbDatabase::database());
    sync->setCollection(COLL);
    QBsonObject hints(
                "$orderby", QBsonObject("marker", 1));
    sync->setHints(hints);
    sync->fetch();
    sync->model()->update("string", "neu", 0);
    EXPECT_EQ("neu", sync->model()->row(0).value("string").toString());
    sync->fetch();
    EXPECT_EQ("neu", sync->model()->row(0).value("string").toString());
    delete sync;
}
TEST_F(QBsonModelCollectionSyncTest, TestItemRemove)
{
    QEjdbCollectionSync *sync = new QEjdbCollectionSync(
                QEjdbDatabase::database());
    sync->setCollection(COLL);
    QBsonObject hints(
                "$orderby", QBsonObject("marker", 1));
    sync->setHints(hints);
    sync->fetch();
    EXPECT_EQ(1, sync->model()->row(0).value("marker").toInt());
    sync->model()->remove(0);
    sync->fetch();
    EXPECT_EQ(2, sync->model()->row(0).value("marker").toInt());
    delete sync;
}

