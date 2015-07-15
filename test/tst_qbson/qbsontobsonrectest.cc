#include "gtest/gtest.h"
#include "qbson/qbsontobsonrec.h"
#include "qbson/qbsonobject_p.h"
#include "qbson/qbsonarray.h"
#include <QByteArray>
TEST(QBsonToBsonRecTest, TestFromQBsonObjectWithId)
{
    bson bsrec;
    bson_type bstype;
    QBsonOid oid = QBsonOid::generate();
    QBsonObject object;
    object.insert("key", "test");
    object.setOid(oid);
    bsrec = convertQBsonToBsonRec(object, 0);
    bson_iterator it;
    BSON_ITERATOR_FROM_BUFFER(&it, bsrec.data);
    bstype = bson_iterator_next(&it);
    EXPECT_EQ("_id", QString(BSON_ITERATOR_KEY(&it)));
    EXPECT_EQ(BSON_OID, bstype);
    bstype = bson_iterator_next(&it);
    EXPECT_EQ(BSON_STRING, bstype);
    EXPECT_EQ("key", QString(BSON_ITERATOR_KEY(&it)));
    EXPECT_EQ("test", QString(bson_iterator_string(&it)));
    bson_destroy(&bsrec);
}
TEST(QBsonToBsonRecTest, TestFromQBsonObjectDataWithId)
{
    bson bsrec;
    bson_type bstype;
    QBsonOid oid = QBsonOid::generate();
    QBsonObjectData *objectData = new QBsonObjectData;
    objectData->insert("key", "test");
    objectData->oid = oid;
    bsrec = convertQBsonToBsonRec(objectData, 0);
    bson_iterator it;
    BSON_ITERATOR_FROM_BUFFER(&it, bsrec.data);
    bstype = bson_iterator_next(&it);
    EXPECT_EQ("_id", QString(BSON_ITERATOR_KEY(&it)));
    EXPECT_EQ(BSON_OID, bstype);
    bstype = bson_iterator_next(&it);
    EXPECT_EQ(BSON_STRING, bstype);
    EXPECT_EQ("key", QString(BSON_ITERATOR_KEY(&it)));
    EXPECT_EQ("test", QString(bson_iterator_string(&it)));
    bson_destroy(&bsrec);
    delete objectData;
}

TEST(QBsonToBsonRecTest, TestFromQBsonOjectData)
{
    bson bsrec;
    bson_type bstype;
    QBsonObjectData *objectData = new QBsonObjectData;
    objectData->insert("key", "test");
    bsrec = convertQBsonToBsonRec(objectData, 0);
    bson_iterator it;
    BSON_ITERATOR_FROM_BUFFER(&it, bsrec.data);
    bstype = bson_iterator_next(&it);
    EXPECT_EQ(BSON_STRING, bstype);
    EXPECT_EQ("key", QString(BSON_ITERATOR_KEY(&it)));
    EXPECT_EQ("test", QString(bson_iterator_string(&it)));
    bson_destroy(&bsrec);
    delete objectData;
}
TEST(QBsonToBsonRecTest, TestFromQBsonWithBinary)
{
    bson bsrec;
    bson_type bstype;
    QBsonObject object;
    object.append("key", QByteArray(5,'c'));
    bsrec = convertQBsonToBsonRec(object, 0);
    bson_iterator it;
    BSON_ITERATOR_FROM_BUFFER(&it, bsrec.data);
    bstype = bson_iterator_next(&it);
    EXPECT_EQ(BSON_BINDATA, bstype);
    EXPECT_EQ("key", QString(BSON_ITERATOR_KEY(&it)));
    EXPECT_EQ(5, bson_iterator_bin_len(&it));
    EXPECT_EQ(QString("ccccc"), QString(bson_iterator_bin_data(&it)));
    bson_destroy(&bsrec);
}
TEST(QBsonToBsonRecTest, TestFromQBsonWithString)
{
    bson bsrec;
    bson_type bstype;
    QBsonObject object;
    object.append("key", "test");
    bsrec = convertQBsonToBsonRec(object, 0);
    bson_iterator it;
    BSON_ITERATOR_FROM_BUFFER(&it, bsrec.data);
    bstype = bson_iterator_next(&it);
    EXPECT_EQ(BSON_STRING, bstype);
    EXPECT_EQ("key", QString(BSON_ITERATOR_KEY(&it)));
    EXPECT_EQ("test", QString(bson_iterator_string(&it)));
    bson_destroy(&bsrec);
}
TEST(QBsonToBsonRecTest, TestFromQBsonWithDouble)
{
    bson bsrec;
    bson_type bstype;
    QBsonObject object;
    object.append("key", 11.11);
    bsrec = convertQBsonToBsonRec(object, 0);
    bson_iterator it;
    BSON_ITERATOR_FROM_BUFFER(&it, bsrec.data);
    bstype = bson_iterator_next(&it);
    EXPECT_EQ(BSON_DOUBLE, bstype);
    EXPECT_EQ("key", QString(BSON_ITERATOR_KEY(&it)));
    EXPECT_EQ(11.11, bson_iterator_double(&it));
    bson_destroy(&bsrec);
}
TEST(QBsonToBsonRecTest, TestFromQBsonWithBoolean)
{
    bson bsrec;
    bson_type bstype;
    QBsonObject object;
    object.append("key", true);
    bsrec = convertQBsonToBsonRec(object, 0);
    bson_iterator it;
    BSON_ITERATOR_FROM_BUFFER(&it, bsrec.data);
    bstype = bson_iterator_next(&it);
    EXPECT_EQ(BSON_BOOL, bstype);
    EXPECT_EQ("key", QString(BSON_ITERATOR_KEY(&it)));
    EXPECT_EQ(true, bson_iterator_bool(&it));
    bson_destroy(&bsrec);
}
TEST(QBsonToBsonRecTest, TestFromQBsonWithLong)
{
    bson bsrec;
    bson_type bstype;
    QBsonObject object;
    object.append("key", (qlonglong)11);
    bsrec = convertQBsonToBsonRec(object, 0);
    bson_iterator it;
    BSON_ITERATOR_FROM_BUFFER(&it, bsrec.data);
    bstype = bson_iterator_next(&it);
    EXPECT_EQ(BSON_LONG, bstype);
    EXPECT_EQ("key", QString(BSON_ITERATOR_KEY(&it)));
    EXPECT_EQ(11, bson_iterator_long(&it));
    bson_destroy(&bsrec);
}
TEST(QBsonToBsonRecTest, TestFromQBsonWithInteger)
{
    bson bsrec;
    bson_type bstype;
    QBsonObject object;
    object.append("key", 11);
    bsrec = convertQBsonToBsonRec(object, 0);
    bson_iterator it;
    BSON_ITERATOR_FROM_BUFFER(&it, bsrec.data);
    bstype = bson_iterator_next(&it);
    EXPECT_EQ(BSON_INT, bstype);
    EXPECT_EQ("key", QString(BSON_ITERATOR_KEY(&it)));
    EXPECT_EQ(11, bson_iterator_int(&it));
    bson_destroy(&bsrec);
}
TEST(QBsonToBsonRecTest, TestFromQBsonWithDateTime)
{
    bson bsrec;
    bson_type bstype;
    QBsonObject object;
    QDateTime time = QDateTime::currentDateTime();
    object.append("key", time);
    bsrec = convertQBsonToBsonRec(object, 0);
    bson_iterator it;
    BSON_ITERATOR_FROM_BUFFER(&it, bsrec.data);
    bstype = bson_iterator_next(&it);
    EXPECT_EQ(BSON_DATE, bstype);
    EXPECT_EQ("key", QString(BSON_ITERATOR_KEY(&it)));
    EXPECT_EQ(time.toMSecsSinceEpoch(), bson_iterator_date(&it));
    bson_destroy(&bsrec);
}
TEST(QBsonToBsonRecTest, TestFromQBsonWithOid)
{
    bson bsrec;
    bson_type bstype;
    QBsonObject object;
    QBsonOid oid = QBsonOid::generate();
    object.append("key", oid);
    bsrec = convertQBsonToBsonRec(object, 0);
    bson_iterator it;
    BSON_ITERATOR_FROM_BUFFER(&it, bsrec.data);
    bstype = bson_iterator_next(&it);
    EXPECT_EQ(BSON_OID, bstype);
    EXPECT_EQ("key", QString(BSON_ITERATOR_KEY(&it)));
    char xoid[25];
    bson_oid_to_string(bson_iterator_oid(&it), xoid);
    EXPECT_EQ(oid.toString(), QString(xoid));
    bson_destroy(&bsrec);
}
TEST(QBsonToBsonRecTest, TestFromQBsonWithObject)
{
    bson bsrec;
    bson_type bstype;
    QBsonObject object;
    QBsonObject subobject;
    object.append("key", subobject);
    bsrec = convertQBsonToBsonRec(object, 0);
    bson_iterator it;
    BSON_ITERATOR_FROM_BUFFER(&it, bsrec.data);
    bstype = bson_iterator_next(&it);
    EXPECT_EQ(BSON_OBJECT, bstype);
    EXPECT_EQ("key", QString(BSON_ITERATOR_KEY(&it)));
    bson_destroy(&bsrec);
}
TEST(QBsonToBsonRecTest, TestFromQBsonWithObjectWithValue)
{
    bson bsrec;
    bson_type bstype, bstypesub;
    QBsonObject object;
    QBsonObject subobject;
    subobject.append("subkey", "test");
    object.append("key", subobject);
    bsrec = convertQBsonToBsonRec(object, 0);
    bson_iterator it;
    bson_iterator subit;
    BSON_ITERATOR_FROM_BUFFER(&it, bsrec.data);
    bstype = bson_iterator_next(&it);
    EXPECT_EQ(BSON_OBJECT, bstype);
    bson_iterator_subiterator(&it, &subit);
    bstypesub = bson_iterator_next(&subit);
    EXPECT_EQ(BSON_STRING, bstypesub);
    EXPECT_EQ("key", QString(BSON_ITERATOR_KEY(&it)));
    EXPECT_EQ("subkey", QString(BSON_ITERATOR_KEY(&subit)));
    EXPECT_EQ("test", QString(bson_iterator_string(&subit)));
    bson_destroy(&bsrec);
}
TEST(QBsonToBsonRecTest, TestFromQBsonWithArrayWithValue)
{
    bson bsrec;
    bson_type bstype, bstypesub;
    QBsonObject object;
    QBsonArray subarray;
    subarray.append("test");
    object.append("key", subarray);
    bsrec = convertQBsonToBsonRec(object, 0);
    bson_iterator it;
    bson_iterator subit;
    BSON_ITERATOR_FROM_BUFFER(&it, bsrec.data);
    bstype = bson_iterator_next(&it);
    EXPECT_EQ(BSON_ARRAY, bstype);
    bson_iterator_subiterator(&it, &subit);
    bstypesub = bson_iterator_next(&subit);
    EXPECT_EQ(BSON_STRING, bstypesub);
    EXPECT_EQ("key", QString(BSON_ITERATOR_KEY(&it)));
    EXPECT_EQ("test", QString(bson_iterator_string(&subit)));
    bson_destroy(&bsrec);
}
