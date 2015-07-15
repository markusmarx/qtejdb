
#include "gtest/gtest.h"
#include "bson.h"
#include "qbson/qbsonobject.h"
#include "qbson/qbsonobject_p.h"
#include "qbson/qbsonarray.h"
#include "qbson/qbsonfrombsonrec.h"
#include <QDateTime>
TEST(QBsonFromBsonRec, TestWithBsonObjectDataWithEmptyBson)
{
    QBsonObjectData *objectData = new QBsonObjectData;
    convertBsonObjectFromBsonRec(0, objectData);
    EXPECT_TRUE(objectData->values.isEmpty());
    delete objectData;
}
TEST(QBsonFromBsonRec, TestWithBsonObjectData)
{
    QBsonObjectData *objectData = new QBsonObjectData;
    bson bsrec;
    bson_init(&bsrec);
    bson_append_string(&bsrec, "test", "test");
    bson_finish(&bsrec);
    convertBsonObjectFromBsonRec(&bsrec, objectData);
    EXPECT_EQ("test", objectData->values.value("test"));
    delete objectData;
}
TEST(QBsonFomBsonRec, TestFromBsonRecWithEmptyBson)
{
    QBsonObject object;
    convertBsonObjectFromBsonRec(0, object);
    EXPECT_TRUE(object.isEmpty());
}
TEST(QBsonFromBsonRec, TestFromBsonRecWithString)
{
    bson bsrec;
    bson_init(&bsrec);
    bson_append_string(&bsrec, "test", "test");
    bson_finish(&bsrec);
    QBsonObject object;
    convertBsonObjectFromBsonRec(&bsrec, object);
    EXPECT_TRUE(object.value("test").type() == QBsonValue::String);
    EXPECT_EQ("test", object.value("test"));
    bson_destroy(&bsrec);
}
TEST(QBsonFromBsonRec, TestFromBsonRecWithInteger)
{
    bson bsrec;
    bson_init(&bsrec);
    bson_append_int(&bsrec, "test", 11);
    bson_finish(&bsrec);
    QBsonObject object;
    convertBsonObjectFromBsonRec(&bsrec, object);
    EXPECT_TRUE(object.value("test").type() == QBsonValue::Integer);
    EXPECT_EQ(11, object.value("test"));
    bson_destroy(&bsrec);
}
TEST(QBsonFromBsonRec, TestFromBsonRecWithBoolean)
{
    bson bsrec;
    bson_init(&bsrec);
    bson_append_bool(&bsrec, "test", true);
    bson_finish(&bsrec);
    QBsonObject object;
    convertBsonObjectFromBsonRec(&bsrec, object);
    EXPECT_TRUE(object.value("test").type() == QBsonValue::Bool);
    EXPECT_EQ(true, object.value("test"));
    bson_destroy(&bsrec);
}
TEST(QBsonFromBsonRec, TestFromBsonRecWithDouble)
{
    bson bsrec;
    bson_init(&bsrec);
    bson_append_double(&bsrec, "test", 11.11);
    bson_finish(&bsrec);
    QBsonObject object;
    convertBsonObjectFromBsonRec(&bsrec, object);
    EXPECT_TRUE(object.value("test").type() == QBsonValue::Double);
    EXPECT_EQ(11.11, object.value("test"));
    bson_destroy(&bsrec);
}
TEST(QBsonFromBsonRec, TestFromBsonRecWithLong)
{
    bson bsrec;
    bson_init(&bsrec);
    bson_append_long(&bsrec, "test", (long)11);
    bson_finish(&bsrec);
    QBsonObject object;
    convertBsonObjectFromBsonRec(&bsrec, object);
    EXPECT_TRUE(object.value("test").type() == QBsonValue::Long);
    EXPECT_EQ(11, object.value("test"));
    bson_destroy(&bsrec);
}
TEST(QBsonFromBsonRec, TestFromBsonRecWithDateTime)
{
    bson bsrec;
    bson_init(&bsrec);
    QDateTime time = QDateTime::currentDateTime();
    bson_append_date(&bsrec, "test", time.toMSecsSinceEpoch());
    bson_finish(&bsrec);
    QBsonObject object;
    convertBsonObjectFromBsonRec(&bsrec, object);
    EXPECT_TRUE(object.value("test").type() == QBsonValue::DateTime);
    EXPECT_EQ(time, object.value("test"));
    bson_destroy(&bsrec);
}
TEST(QBsonFromBsonRec, TestFromBsonRecWithObject)
{
    bson bsrec;
    bson_init(&bsrec);
    bson_append_start_object(&bsrec, "test");
    bson_append_finish_object(&bsrec);
    bson_finish(&bsrec);
    QBsonObject object;
    convertBsonObjectFromBsonRec(&bsrec, object);
    EXPECT_TRUE(object.value("test").type() == QBsonValue::Object);
    EXPECT_TRUE(object.value("test").toObject().isEmpty());
    bson_destroy(&bsrec);
}
TEST(QBsonFromBsonRec, TestFromBsonRecWithArray)
{
    bson bsrec;
    bson_init(&bsrec);
    bson_append_start_array(&bsrec, "test");
    bson_append_finish_array(&bsrec);
    bson_finish(&bsrec);
    QBsonObject object;
    convertBsonObjectFromBsonRec(&bsrec, object);
    EXPECT_TRUE(object.value("test").type() == QBsonValue::Array);
    EXPECT_TRUE(object.value("test").toArray().size() == 0);
    bson_destroy(&bsrec);
}
TEST(QBsonFromBsonRec, TestFromBsonRecWithObjectAndSubvalue)
{
    bson bsrec;
    bson_init(&bsrec);
    bson_append_start_object(&bsrec, "test");
    bson_append_string(&bsrec, "subtest", "test");
    bson_append_finish_object(&bsrec);
    bson_finish(&bsrec);
    QBsonObject object;
    convertBsonObjectFromBsonRec(&bsrec, object);
    EXPECT_TRUE(object.value("test").type() == QBsonValue::Object);
    EXPECT_FALSE(object.value("test").toObject().isEmpty());
    EXPECT_EQ("test",
              object.value("test").toObject().value("subtest"));
    bson_destroy(&bsrec);
}
TEST(QBsonFromBsonRec, TestFromBsonRecWithArrayWithValue)
{
    bson bsrec;
    bson_init(&bsrec);
    bson_append_start_array(&bsrec, "test");
    bson_append_string(&bsrec, "subtest", "test");
    bson_append_finish_array(&bsrec);
    bson_finish(&bsrec);
    QBsonObject object;
    convertBsonObjectFromBsonRec(&bsrec, object);
    EXPECT_TRUE(object.value("test").type() == QBsonValue::Array);
    EXPECT_FALSE(object.value("test").toArray().size() == 0);
    EXPECT_EQ("test",
              object.value("test").toArray().value(0));
    bson_destroy(&bsrec);
}
TEST(QBsonFromBsonRec, TestFromBsonRecWithOid)
{
    bson bsrec;
    bson_oid_t oid_t;
    bson_init(&bsrec);
    bson_oid_gen(&oid_t);
    bson_append_oid(&bsrec, "test", &oid_t);
    bson_finish(&bsrec);
    QBsonObject object;
    convertBsonObjectFromBsonRec(&bsrec, object);
    EXPECT_TRUE(object.value("test").type() == QBsonValue::Id);
    char oid_str[25];
    bson_oid_to_string(&oid_t, oid_str);
    EXPECT_EQ(QString(oid_str),
              object.value("test").toId().toString());
    bson_destroy(&bsrec);
}
TEST(QBsonFromBsonRec, TestFromBsonRecWithId)
{
    bson bsrec;
    bson_oid_t bson_oid;
    bson_init(&bsrec);
    bson_oid_gen(&bson_oid);
    bson_append_oid(&bsrec, "_id", &bson_oid);
    bson_finish(&bsrec);
    QBsonObject object;
    convertBsonObjectFromBsonRec(&bsrec, object);
    EXPECT_TRUE(object.hasOid());
    EXPECT_TRUE(object.oid().isValid());
    char strOid[25];
    bson_oid_to_string(&bson_oid, strOid);
    EXPECT_EQ(QBsonOid(strOid), object.oid());
    bson_destroy(&bsrec);
}
TEST(QBsonFromBsonRec, TestFromBsonRecToBsonDataWithId)
{
    bson bsrec;
    bson_oid_t bson_oid;
    bson_init(&bsrec);
    bson_oid_gen(&bson_oid);
    bson_append_oid(&bsrec, "_id", &bson_oid);
    bson_finish(&bsrec);
    QBsonObjectData *objectData = new QBsonObjectData;
    convertBsonObjectFromBsonRec(&bsrec, objectData);
    EXPECT_TRUE(objectData->oid.isValid());
    char strOid[25];
    bson_oid_to_string(&bson_oid, strOid);
    EXPECT_EQ(QBsonOid(strOid), objectData->oid);
    bson_destroy(&bsrec);
}
