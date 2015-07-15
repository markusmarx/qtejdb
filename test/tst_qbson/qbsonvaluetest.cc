#include "gtest/gtest.h"
#include "qbson/qbsonvalue.h"
#include <QDateTime>
#include "qbson/qbsonoid.h"
#include "qbson/qbsonobject.h"
#include "qbson/qbsonarray.h"
#include "qbytearray.h"

TEST(QBsonValueTest, TestFromVariantWithOid)
{
    EXPECT_TRUE(QBsonValue::fromVariant(
                    QVariant::fromValue<QBsonOid>(QBsonOid())
                ).type() == QBsonValue::Id);
}
TEST(QBsonValueTest, TestFromVariantWithArray)
{
    EXPECT_TRUE(QBsonValue::fromVariant(
                    QVariant::fromValue<QBsonArray>(QBsonArray())
                ).type() == QBsonValue::Array);
}
TEST(QBsonValueTest, TestFromVariantWithObject)
{
    EXPECT_TRUE(QBsonValue::fromVariant(
                    QVariant::fromValue<QBsonObject>(QBsonObject())
                ).type() == QBsonValue::Object);
}
TEST(QBsonValueTest, TestFromVariantWithDateTime)
{
    EXPECT_TRUE(QBsonValue::fromVariant(QDateTime::currentDateTime()).type()
                == QBsonValue::DateTime);
}
TEST(QBsonValueTest, TestFromVariantWithDouble)
{
    EXPECT_TRUE(QBsonValue::fromVariant(11.11).type() == QBsonValue::Double);
}
TEST(QBsonValueTest, TestFromVariantWithInt)
{
    EXPECT_TRUE(QBsonValue::fromVariant(11).type() == QBsonValue::Integer);
}
TEST(QBsonValueTest, TestFromVariantWithLong)
{
    EXPECT_TRUE(QBsonValue::fromVariant((qlonglong)11).type() == QBsonValue::Long);
}
TEST(QBsonValueTest, TestFromVariantWithString)
{
    EXPECT_TRUE(QBsonValue::fromVariant("test").type() == QBsonValue::String);
}
TEST(QBsonValueTest, TestFromVariantWithBinary)
{
    EXPECT_TRUE(QBsonValue::fromVariant(QByteArray()).type() == QBsonValue::Binary);
}
TEST(QBsonValueTest, TestFromVariantWithBool)
{
    EXPECT_TRUE(QBsonValue::fromVariant(QVariant(true)).toBool());
}
TEST(QBsonValueTest, TestFromVariantWithInvalid)
{
    EXPECT_FALSE(QBsonValue::fromVariant(QVariant()).isValid());
}
TEST(QBsonValueTest, TestOperatorEqual)
{
    QBsonValue value("test");
    EXPECT_TRUE(value == QVariant("test"));
}
TEST(QBsonValueTest, TestToConverterWithInvalid)
{
    QBsonValue value;
    EXPECT_EQ(0, value.toLong());
    EXPECT_EQ(0, value.toInt());
    EXPECT_EQ(0, value.toDouble());
    EXPECT_EQ(0, value.toBinary().size());
    EXPECT_TRUE(value.toObject().isEmpty());
    EXPECT_FALSE(value.toDateTime().isValid());
    EXPECT_EQ(0, value.toArray().size());
    EXPECT_FALSE(value.toBool());
    EXPECT_FALSE(value.toId().isValid());

}
TEST(QBsonValueTest, TestToLongAndToIntWithDouble)
{
    QBsonValue value(11.11);
    EXPECT_EQ(11, value.toLong());
    EXPECT_EQ(11, value.toInt());
}
TEST(QBsonValueTest, TestToDoubleWithLong)
{
    QBsonValue value((qlonglong)11);
    EXPECT_EQ(11, value.toDouble());
}
TEST(QBsonValueTest, TestToStringWithNumber)
{
    QBsonValue value(22);
    EXPECT_EQ("22", value.toString());
}
TEST(QBsonValueTest, TestToStringWithInvalid)
{
    QBsonValue value;
    EXPECT_TRUE(value.toString().isEmpty());
}
TEST(QBsonValueTest, TestAssign)
{
    QBsonValue value("test"), value2;
    value2 = value;

    EXPECT_EQ(value, value2);
}
TEST(QBsonValueTest, TestEmpty)
{
    QBsonValue value;
    EXPECT_TRUE(value.type() == QBsonValue::Empty);
}
TEST(QBsonValueTest, TestArray)
{
    QBsonArray array;
    array.append("test");
    QBsonValue value(array);

    EXPECT_TRUE(value.type() == QBsonValue::Array);
    array = value.toArray();
    EXPECT_TRUE(value.isArray());
    EXPECT_EQ(1, array.size());
}
TEST(QBsonValueTest, TestObject)
{
    QBsonObject object;
    QBsonValue value(object);
    EXPECT_TRUE(value.isObject());
    value.toObject();
}
TEST(QBsonValueTest, TestOid)
{
    QBsonOid oid;
    QBsonValue value(oid);
    EXPECT_TRUE(value.type() == QBsonValue::Id);
    EXPECT_EQ(oid, value.toId());
    EXPECT_EQ(oid.toString(), value.toString());
}
TEST(QBsonValueTest, TestLong)
{
    QBsonValue value((qlonglong) 77);
    EXPECT_TRUE(value.type() == QBsonValue::Long);
    EXPECT_EQ((qlonglong) 77, value);
}
TEST(QBsonValueTest, TestDateTime)
{
    QDateTime time = QDateTime::currentDateTime();
    QBsonValue value(time);
    EXPECT_TRUE(value.type() == QBsonValue::DateTime);
    EXPECT_EQ(time, value);
}
TEST(QBsonValueTest, TestDouble)
{
    QBsonValue value(6.7);
    EXPECT_TRUE(value.type() == QBsonValue::Double);
    EXPECT_EQ(6.7, value);
}
TEST(QBsonValueTest, TestBoolean)
{
    QBsonValue value(true);
    EXPECT_TRUE(value.type() == QBsonValue::Bool);
    EXPECT_EQ(true, value);
}
TEST(QBsonValueTest, TestChar)
{
    QBsonValue value("test");
    EXPECT_TRUE(value.type() == QBsonValue::String);
    EXPECT_EQ(QString("test"), value);
    EXPECT_EQ("test", value);
    EXPECT_EQ(QBsonValue("test"), value);
}
TEST(QBsonValueTest, TestQString)
{
    QBsonValue value(QString("test"));
    EXPECT_TRUE(value.type() == QBsonValue::String);
    EXPECT_EQ(QString("test"), value);
    EXPECT_EQ("test", value);
    EXPECT_EQ(QBsonValue("test"), value);
}
TEST(QBsonValueTest, TestBinary)
{
    QByteArray byteArray;
    QBsonValue value(byteArray);
    EXPECT_TRUE(value.type() == QBsonValue::Binary);
    EXPECT_EQ(0, value.toBinary().size());
}
TEST(QBsonValueTest, TestIntegers)
{
    QBsonValue value(1);
    EXPECT_TRUE(value.type() == QBsonValue::Integer);
    EXPECT_EQ(1, value);
    EXPECT_EQ(QBsonValue(1), value);
}
