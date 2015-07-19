#include "gtest/gtest.h"
#include "itemmodel/qejdbitemmodel.h"
#include "itemmodel/qejdbcollectionsync.h"
#include "itemmodel/qejdbarraypropertysync.h"
#include "../helper.h"

#define MODE QEJDB::CREATE | QEJDB::WRITE | \
    QEJDB::SYNC | QEJDB::LOCK_NB | QEJDB::TRUNCATE

#define DBURL "file:test_qejdbitemmodel.db"
#define COLL "testCollection"
#define COLL2 "testCollection1"
#define PROP "arrayProperty"


class QEjdbItemModelTest : public ::testing::Test
{
    protected:

        static void SetUpTestCase()
        {
            QEjdbDatabase::addDatabase(DBURL, MODE);
            QEjdbDatabase::database().createCollection(COLL);
            QEjdbDatabase::database().createCollection(COLL2);

            QBsonArray array;
            for (int i = 1; i < 11; i++) {
                QBsonObject obj = createBsonObject(false, i);
                QEjdbDatabase::database().save(COLL, obj);
                array.append(obj.oid());
            }

            QBsonObject object;
            object.insert(PROP, array);
            QEjdbDatabase::database().save(COLL2, object);
            QEjdbItemModelTest::object = object;
        }

        static void TearDownTestCase()
        {
            QEjdbDatabase::database().removeCollection(COLL);
            QEjdbDatabase::database().removeCollection(COLL2);
            QEjdbDatabase::removeDatabase();
        }

        static QBsonObject object;
};

QBsonObject QEjdbItemModelTest::object;

TEST_F(QEjdbItemModelTest, TestMove)
{
    QEjdbItemModel *itemModel
            = new QEjdbItemModel(
                new QEjdbArrayPropertySync(QEjdbDatabase::database(),
                                           COLL2, PROP, COLL, QEjdbItemModelTest::object)
                );
    itemModel->move(1, 0);
    itemModel->reset();
    EXPECT_EQ(2, itemModel->data(itemModel->index(0), itemModel->roleNames().key(QString("marker").toLatin1())).toInt());
    EXPECT_EQ(1, itemModel->data(itemModel->index(1), itemModel->roleNames().key(QString("marker").toLatin1())).toInt());
    delete itemModel;
}

TEST_F(QEjdbItemModelTest, TestCount)
{
    QEjdbItemModel *itemModel
            = new QEjdbItemModel(
                new QEjdbCollectionSync(QEjdbDatabase::database(), COLL)
            );

    EXPECT_EQ(10, itemModel->rowCount());
    delete itemModel;
}
TEST_F(QEjdbItemModelTest, TestHeaderData)
{
    QEjdbItemModel *itemModel
            = new QEjdbItemModel(
                new QEjdbCollectionSync(QEjdbDatabase::database(), COLL)
            );

    EXPECT_FALSE(itemModel->headerData(0, Qt::Horizontal, 1).isValid());
    delete itemModel;
}
TEST_F(QEjdbItemModelTest, TestInsert)
{
    QEjdbItemModel *itemModel
            = new QEjdbItemModel(
                new QEjdbCollectionSync(QEjdbDatabase::database(), COLL, QBsonObject(),
                                        QBsonObject("$orderby", QBsonObject("marker", 1)))
            );

    itemModel->insert(createBsonObject(false, 0), 0);
    itemModel->reset();
    EXPECT_EQ(11, itemModel->rowCount());
    EXPECT_EQ(0, itemModel->data(itemModel->index(0), itemModel->roleNames().key(QString("marker").toLatin1())).toInt());
    itemModel->remove(0);
    delete itemModel;
}
TEST_F(QEjdbItemModelTest, TestRemove)
{
    QEjdbItemModel *itemModel
            = new QEjdbItemModel(
                new QEjdbCollectionSync(QEjdbDatabase::database(), COLL, QBsonObject(),
                                        QBsonObject("$orderby", QBsonObject("marker", 1)))
            );

    itemModel->remove(0);
    itemModel->reset();
    EXPECT_EQ(2, itemModel->data(itemModel->index(0), itemModel->roleNames().key(QString("marker").toLatin1())).toInt());
    itemModel->insert(createBsonObject(false, 1), 0);
    delete itemModel;
}
TEST_F(QEjdbItemModelTest, TestSetData)
{
    QEjdbItemModel *itemModel
            = new QEjdbItemModel(
                new QEjdbCollectionSync(QEjdbDatabase::database(), COLL, QBsonObject(),
                                        QBsonObject("$orderby", QBsonObject("marker", 1)))
            );

    itemModel->setData(itemModel->index(0), QString("neu"), itemModel->roleNames().key(QString("string").toLatin1()));
    EXPECT_EQ("neu", itemModel->data(itemModel->index(0), itemModel->roleNames().key(QString("string").toLatin1())).toString());
    itemModel->reset();
    EXPECT_EQ("neu", itemModel->data(itemModel->index(0), itemModel->roleNames().key(QString("string").toLatin1())).toString());
    delete itemModel;
}
TEST_F(QEjdbItemModelTest, TestSetSync)
{
    QEjdbItemModel *itemModel
            = new QEjdbItemModel();

    EXPECT_EQ(0, itemModel->rowCount());
    itemModel->setSync(new QEjdbArrayPropertySync(QEjdbDatabase::database(),
                                                  COLL2, PROP, COLL, QEjdbItemModelTest::object));
    EXPECT_EQ(10, itemModel->rowCount());
    delete itemModel;
}
