#include "gtest/gtest.h"
#include "itemmodel/qbsonitemmodel_p.h"
#include "qtejdb/qejdbdatabase.h"
#include "../helper.h"
#include "../qsignalmock.h"
#include "gmock/gmock.h"
#include <QString>
#include <QVariant>

#define MODE QEJDB::CREATE | QEJDB::WRITE | \
    QEJDB::SYNC | QEJDB::LOCK_NB | QEJDB::TRUNCATE

#define DBURL "file:test_qbsonmodel.db"
#define COLL "testCollection"
#define MODEL_DATA(model,row, name) model.data(row, model.roles().key(QString(name).toLatin1()))

class QBsonItemModelSignalSpy: public QSignalMock
{
public:
    BEGIN_MOCK_SIGNALS(QBsonItemModelSignalSpy)

    MOCK_SIGNAL1(itemInserted, (uint))
    MOCK_SIGNAL3(itemUpdated, (QString, QVariant, uint))
    MOCK_SIGNAL2(itemRemoved,(uint, QBsonObject))
    MOCK_SIGNAL2(itemMoved, (uint, uint))
    MOCK_SIGNAL0(reset,(void))
};


TEST(QBsonItemModelTest, TestSet)
{
    QBsonItemModel model;
    QList<QBsonObject> objectList
            = createBsonObjects(10);
    QBsonItemModelSignalSpy spy(&model);
    EXPECT_CALL(spy, reset()).Times(1);
    EXPECT_EQ(0, objectList.at(0).value("marker").toInt());
    EXPECT_EQ(1, objectList.at(1).value("marker").toInt());
    model.set(objectList);
    EXPECT_EQ(10, model.count());
    EXPECT_EQ(0, model.row(0).value("marker").toInt());
    EXPECT_EQ(1, model.row(1).value("marker").toInt());
    EXPECT_EQ(0, MODEL_DATA(model, 0, "marker").toInt());
    EXPECT_EQ(1, MODEL_DATA(model, 1, "marker").toInt());
}
TEST(QBsonItemModelTest, TestRoles)
{
    QBsonItemModel model;
    QList<QBsonObject> objectList
            = createBsonObjects(10);
    model.set(objectList);
    EXPECT_EQ(11, model.roles().size());
}
TEST(QBsonItemModelTest, TestItemInsertWithValidRow)
{
    QBsonItemModel model;
    QBsonItemModelSignalSpy spy(&model);
    EXPECT_CALL(spy, itemInserted(0)).Times(1);
    EXPECT_CALL(spy, reset()).Times(1);
    model.insert(createBsonObject(false), 0);
    EXPECT_EQ(1, model.count());
    EXPECT_EQ("test", MODEL_DATA(model, 0, "string"));

}
TEST(QBsonItemModelTest, TestItemInsertWithNegativeRow)
{
    QBsonItemModel model;
    QBsonItemModelSignalSpy spy(&model);
    EXPECT_CALL(spy, itemInserted(-1)).Times(0);
    model.insert(createBsonObject(false), -1);
    EXPECT_EQ(0, model.count());
}
TEST(QBsonItemModelTest, TestItemInsertWithInvalidRow)
{
    QBsonItemModel model;
    QBsonItemModelSignalSpy spy(&model);
    EXPECT_CALL(spy, itemInserted(1)).Times(0);
    model.insert(createBsonObject(false), 1);
    EXPECT_EQ(0, model.count());
}
TEST(QBsonItemModelTest, TestUpdateItem)
{
    QBsonItemModel model;
    model.insert(createBsonObject(false), 0);
    QBsonItemModelSignalSpy spy(&model);
    EXPECT_CALL(spy, itemUpdated(QString("string"), QVariant("neu"), 0)).Times(1);
    model.update("string", "neu", 0);
    EXPECT_EQ("neu", MODEL_DATA(model, 0, "string"));
}
TEST(QBsonItemModelTest, TestUpdateItemWithInvalidPropertyName)
{
    QBsonItemModel model;
    model.insert(createBsonObject(false), 0);
    QBsonItemModelSignalSpy spy(&model);
    EXPECT_CALL(spy, itemUpdated(QString("string_neu"), QVariant("neu"), 0)).Times(1);
    model.update("string_neu", "neu", 0);
}
TEST(QBsonItemModelTest, TestUpdateItemWithNegativeRow)
{
    QBsonItemModel model;
    model.insert(createBsonObject(false), 0);
    QBsonItemModelSignalSpy spy(&model);
    EXPECT_CALL(spy, itemUpdated(QString("string_neu"), QVariant("neu"), -1)).Times(0);
    model.update("string", "neu", -1);
}
TEST(QBsonItemModelTest, TestUpdateItemWithInvalidRow)
{
    QBsonItemModel model;
    model.insert(createBsonObject(false), 0);
    QBsonItemModelSignalSpy spy(&model);
    EXPECT_CALL(spy, itemUpdated(QString("string_neu"), QVariant("neu"), 1)).Times(0);
    model.update("string", "neu", 1);
}
TEST(QBsonItemModelTest, TestAppendItem)
{
    QBsonItemModel model;
    QBsonItemModelSignalSpy spy(&model);
    EXPECT_CALL(spy, itemInserted(0)).Times(1);
    EXPECT_CALL(spy, reset()).Times(1);
    EXPECT_CALL(spy, itemInserted(1)).Times(1);
    model.append(createBsonObject(false));
    model.append(createBsonObject(false));
}
TEST(QBsonItemModelTest, TestMoveTwoItems)
{
    QBsonItemModel model;
    QList<QBsonObject> objectList = createBsonObjects(2);
    model.set(objectList);
    QBsonItemModelSignalSpy spy(&model);
    EXPECT_CALL(spy, itemMoved(1, 0)).Times(1);
    EXPECT_CALL(spy, itemMoved(0, 1)).Times(1);
    model.move(1, 0);
    EXPECT_EQ(2, model.count());
    EXPECT_EQ(1, MODEL_DATA(model, 0, "marker").toInt());
    EXPECT_EQ(0, MODEL_DATA(model, 1, "marker").toInt());
    model.move(0, 1);
    EXPECT_EQ(2, model.count());
    EXPECT_EQ(0, MODEL_DATA(model, 0, "marker").toInt());
    EXPECT_EQ(1, MODEL_DATA(model, 1, "marker").toInt());
}
TEST(QBsonItemModelTest, TestMoveThreeItems)
{
    QBsonItemModel model;
    QList<QBsonObject> objectList = createBsonObjects(3);
    model.set(objectList);
    QBsonItemModelSignalSpy spy(&model);
    EXPECT_CALL(spy, itemMoved(1, 0)).Times(1);
    EXPECT_CALL(spy, itemMoved(0, 1)).Times(1);
    model.move(1, 0);
    EXPECT_EQ(3, model.count());
    EXPECT_EQ(1, MODEL_DATA(model, 0, "marker").toInt());
    EXPECT_EQ(0, MODEL_DATA(model, 1, "marker").toInt());
    EXPECT_EQ(2, MODEL_DATA(model, 2, "marker").toInt());
    model.move(0, 1);
    EXPECT_EQ(3, model.count());
    EXPECT_EQ(0, MODEL_DATA(model, 0, "marker").toInt());
    EXPECT_EQ(1, MODEL_DATA(model, 1, "marker").toInt());
    EXPECT_EQ(2, MODEL_DATA(model, 2, "marker").toInt());
}
TEST(QBsonItemModelTest, TestMoveWithEqualSourceAndDestination)
{
    QBsonItemModel model;
    QList<QBsonObject> objectList = createBsonObjects(2);
    model.set(objectList);
    QBsonItemModelSignalSpy spy(&model);
    EXPECT_CALL(spy, itemMoved(1, 0)).Times(0);
    model.move(1, 1);
    EXPECT_EQ(2, model.count());
    EXPECT_EQ(0, MODEL_DATA(model, 0, "marker").toInt());
    EXPECT_EQ(1, MODEL_DATA(model, 1, "marker").toInt());
}
TEST(QBsonItemModelTest, TestRemoveWithValid)
{
    QBsonItemModel model;
    QList<QBsonObject> objectList = createBsonObjects(2);
    QBsonObject removedObject = objectList.at(0);
    model.set(objectList);
    QBsonItemModelSignalSpy spy(&model);
    EXPECT_CALL(spy, itemRemoved(0, removedObject)).Times(1);
    model.remove(0);
    EXPECT_EQ(1, model.count());
    EXPECT_EQ(1, MODEL_DATA(model, 0, "marker").toInt());
}
TEST(QBsonItemModelTest, TestRemoveWithInValid)
{
    QBsonItemModel model;
    QList<QBsonObject> objectList = createBsonObjects(2);
    model.set(objectList);
    QBsonItemModelSignalSpy spy(&model);
    model.remove(-1);
    EXPECT_EQ(2, model.count());
    EXPECT_EQ(0, MODEL_DATA(model, 0, "marker").toInt());
}
TEST(QBsonItemModelTest, TestRowWithInvalidIndex)
{
    QBsonItemModel model;
    QList<QBsonObject> objectList = createBsonObjects(2);
    model.set(objectList);
    EXPECT_TRUE(model.row(-1).isEmpty());
    EXPECT_TRUE(model.row(3).isEmpty());
}
TEST(QBsonItemModelTest, TestDataWithInvaidRow)
{
    QBsonItemModel model;
    QList<QBsonObject> objectList = createBsonObjects(2);
    model.set(objectList);
    EXPECT_FALSE(MODEL_DATA(model, -1, "string").isValid());
    EXPECT_FALSE(MODEL_DATA(model, 3, "string").isValid());
}
TEST(QBsonItemModelTest, TestSetDataWithValidRowIndex)
{
    QBsonItemModel model;
    QList<QBsonObject> objectList = createBsonObjects(2);
    model.set(objectList);
    QBsonItemModelSignalSpy spy(&model);
    EXPECT_CALL(spy, itemUpdated(QString("string"), QVariant("neu"), 0)).Times(1);
    model.setData(0, model.roles().key(QString("string").toLatin1()), QBsonValue("neu"));
    EXPECT_EQ("neu", MODEL_DATA(model, 0, "string"));
}
TEST(QBsonItemModelTest, TestSetDataWithInValidRowIndex)
{
    QBsonItemModel model;
    QList<QBsonObject> objectList = createBsonObjects(2);
    model.set(objectList);
    QBsonItemModelSignalSpy spy(&model);
    EXPECT_CALL(spy, itemUpdated(QString("string"), QVariant("neu"), 0)).Times(0);
    model.setData(-1, model.roles().key(QString("string").toLatin1()), QBsonValue("neu"));
}
TEST(QBsonItemModelTest, TestRoleAsKeyWithInvalidRole)
{
    QBsonItemModel model;
    QList<QBsonObject> objectList = createBsonObjects(2);
    model.set(objectList);
    QBsonItemModelSignalSpy spy(&model);
    EXPECT_CALL(spy, itemUpdated(QString("string"), QVariant("neu"), 0)).Times(0);
    model.setData(0, -1, QBsonValue("neu"));
}
TEST(QBsonItemModelTest, TestGetByOid)
{
    QBsonItemModel model;
    QList<QBsonObject> objectList = createBsonObjects(2);
    model.set(objectList);
    EXPECT_EQ(objectList.at(0).oid(), model.byOid(objectList.at(0).oid()).oid());
}
TEST(QBsonItemModelTest, TestGetByOidWithInvalidOid)
{
    QBsonItemModel model;
    QList<QBsonObject> objectList = createBsonObjects(2);
    model.set(objectList);
    EXPECT_TRUE(model.byOid("").isEmpty());
}
TEST(QBsonItemModelTest, TestClear)
{
    QBsonItemModel model;
    QList<QBsonObject> objectList = createBsonObjects(2);
    model.set(objectList);
    EXPECT_EQ(2, model.count());
    model.clear();
    EXPECT_EQ(0, model.count());
}
