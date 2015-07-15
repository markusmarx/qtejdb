#include "gtest/gtest.h"
#include "qbson/qbsonobject.h"
#include "bson.h"
#include <QDataStream>

TEST(QBsonObjectTest, TestInvalidValue)
{
    QBsonObject obj;
    EXPECT_FALSE(obj.value("test").isValid());
}
TEST(QBsonObjectTest, TestContains)
{
    QBsonObject obj;
    obj.append("test", "test");
    EXPECT_TRUE(obj.contains("test"));
    EXPECT_TRUE(obj.contains(QString("test")));
}
TEST(QBsonObjectTest, TestNames)
{
    QBsonObject obj;
    obj.append("name1", "test").append("name2", 1);
    EXPECT_EQ(2, obj.names().count());
    EXPECT_EQ("name1", obj.names().at(0));
    EXPECT_EQ("name2", obj.names().at(1));
}
TEST(QBsonObjectTest, TestRemove)
{
    QBsonObject obj;
    obj.append("name1", "test");
    obj.remove("name1");
    EXPECT_TRUE(obj.isEmpty());
}
TEST(QBsonObjectTest, TestToBinary)
{
    QBsonObject obj;
    bson_type bstype;
    bson_iterator it;
    bson bsrec;

    obj.append("key", "test");
    QByteArray binary = obj.toBinary();
    bson_init_with_data(&bsrec, binary.constData());
    bson_finish(&bsrec);
    BSON_ITERATOR_FROM_BUFFER(&it, bsrec.data);
    bstype = bson_iterator_next(&it);
    EXPECT_EQ(BSON_STRING, bstype);
    EXPECT_EQ("key", QString(BSON_ITERATOR_KEY(&it)));
    EXPECT_EQ("test", QString(bson_iterator_string(&it)));
}
TEST(QBsonObjectTest, TestFromBinary)
{
    QBsonObject obj;
    bson_type bstype;
    bson_iterator it;
    const char* binary;
    bson bsrec;
    int size;
    bson_init(&bsrec);
    bson_append_string(&bsrec, "test", "test");
    bson_finish(&bsrec);
    binary = bson_data(&bsrec);
    size = bson_size(&bsrec);
    obj = QBsonObject(QByteArray(binary, size));
    EXPECT_EQ("test", obj.value("test"));
}
TEST(QBsonObjectTest, TestConstructorWithKeyValue)
{
    QBsonObject obj("key", "value");
    EXPECT_EQ("value", obj.value("key"));
}
TEST(QBsonObjectTest, TestOid)
{
    QBsonObject object;
    QBsonOid oid = QBsonOid::generate();
    EXPECT_FALSE(object.hasOid());
    EXPECT_FALSE(object.oid().isValid());
    object.setOid(oid);
    EXPECT_TRUE(object.hasOid());
    EXPECT_TRUE(object.oid().isValid());
    EXPECT_EQ(oid, object.oid());
}
TEST(QBsonObjectTest, TestWriteWriteAndReadFromDataStream)
{
    QBsonObject obj, obj2;
    QByteArray buffer;

    obj.append("key", "test");

    QDataStream dw(&buffer, QIODevice::WriteOnly);
    dw << obj;
    QDataStream dr(&buffer, QIODevice::ReadOnly);
    dr >> obj2;

    EXPECT_EQ("test", obj2.value("key"));
}
TEST(QBsonObjectTest, TestWriteWriteAndReadListsFromDataStream)
{
    QBsonObject obj, obj2;
    QByteArray buffer;
    obj.append("key", "test");
    obj2.append("key", "test2");

    QList<QBsonObject> list, list2;
    list.append(obj);
    list.append(obj2);

    QDataStream dw(&buffer, QIODevice::WriteOnly);
    dw << list;
    QDataStream dr(&buffer, QIODevice::ReadOnly);
    dr >> list2;

    EXPECT_EQ(2, list.size());
    EXPECT_EQ("test", list.at(0).value("key"));
    EXPECT_EQ("test2", list.at(1).value("key"));
}
