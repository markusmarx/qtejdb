#include "gtest/gtest.h"
#include "qbson/qbsonoid.h"
#include <QHash>

TEST(QBsonOidTest, TestOidGeneration)
{
    QBsonOid oid = QBsonOid::generate();
    EXPECT_NE(  QString("000000000000000000000000"),
                oid.toString()
                );
    EXPECT_TRUE(oid.isValid());

}
TEST(QBsonOidTest, TestValidOid)
{
    EXPECT_TRUE(QBsonOid("110000000000000000000011").isValid());
}
TEST(QBsonOidTest, TestInvalidOid)
{
    EXPECT_FALSE(QBsonOid("-1").isValid());
    EXPECT_FALSE(QBsonOid().isValid());
}
TEST(QBsonOidTest, TestAssign)
{
    QBsonOid oid = QBsonOid::generate(), oid2;
    oid2 = oid;
    EXPECT_EQ(oid, oid2);
}
TEST(QBsonOidTest, TestConstructWithQLatin1String)
{
    QBsonOid oid(QLatin1String("000000000000000000000000"));
    EXPECT_EQ(QString("000000000000000000000000"), oid.toString());
}
TEST(QBsonOidTest, TestHash)
{
    QBsonOid oid("000000000000000000000000");
    EXPECT_EQ(qHash(QString("000000000000000000000000")), oid.hash());
}
TEST(QBsonOidTest, TestCastToQString)
{
    QBsonOid oid("000000000000000000000000");
    EXPECT_EQ(QString("000000000000000000000000"), (QString)oid);
}
