#include "gtest/gtest.h"
#include "qbson/qbsonarray.h"
#include "qbson/qbsonobject.h"

TEST(QBsonArrayTest, TestInsertValue)
{
    QBsonArray array;
    array.insert(0, "1");
    EXPECT_EQ("1", array.value(0));
}
TEST(QBsonArrayTest, TestInsertAndGetValueWithOuterRangeIndex)
{
    QBsonArray array;
    array.insert(1, "1");
    EXPECT_EQ(0, array.size());
    EXPECT_FALSE(array.value(0).isValid());
}
TEST(QBsonArrayTest, TestRemoveValue)
{
    QBsonArray array;
    array.insert(0, "1");
    array.remove(0);
    EXPECT_EQ(0, array.size());
}
TEST(QBsonArrayTest, TestRemoveValueWithOuterRangeIndex)
{
    QBsonArray array;
    array.insert(0, "1");
    array.remove(1);
    EXPECT_EQ(1, array.size());
}
TEST(QBsonArrayTest, TestTakeValue)
{
    QBsonArray array;
    array.insert(0, "1");
    QBsonValue value = array.take(0);
    EXPECT_EQ(0, array.size());
    EXPECT_EQ("1", value);
}
TEST(QBsonArrayTest, TestTakeValueWithOuterRangeIndex)
{
    QBsonArray array;
    array.insert(0, "1");
    QBsonValue value = array.take(1);
    EXPECT_EQ(1, array.size());
    EXPECT_FALSE(value.isValid());
}
TEST(QBsonArrayTest, TestCollectProperties)
{
    QBsonObject ob, ob1;
    ob.append("test", 1);
    ob1.append("test", 2);
    QBsonArray array;
    array.append(ob).append(ob1);
    QList<QBsonValue> collection = array.collectProperties("test");
    EXPECT_EQ(2, collection.size());
    EXPECT_EQ(1, collection.at(0));
    EXPECT_EQ(2, collection.at(1));
}
TEST(QBsonArrayTest, TestReplaceValue)
{
    QBsonArray array;
    array.insert(0, "1");
    array.replace(0, "2");
    EXPECT_EQ(1, array.size());
    EXPECT_EQ("2", array.value(0));
}
TEST(QBsonArrayTest, TestReplaceValueValueWithOuterRangeIndex)
{
    QBsonArray array;
    array.insert(0, "1");
    array.replace(1, "2");
    EXPECT_EQ(1, array.size());
    EXPECT_EQ("1", array.value(0));
}
