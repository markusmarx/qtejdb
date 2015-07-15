#include "gtest/gtest.h"
#include "qtejdb/qejdbresult.h"
#include "qtejdb/qejdbdatabase.h"
#include <QDebug>

#define MODE QEJDB::CREATE | QEJDB::WRITE | \
    QEJDB::SYNC | QEJDB::LOCK_NB | QEJDB::TRUNCATE

#define DBURL "file:test_result.db"
#define COLL "testCollection"

QEjdbResult checkAssignment(const QEjdbResult &result)
{
    return result;
}

class QEjdbResultTest : public ::testing::Test
{
    protected:

        static void SetUpTestCase()
        {
            QEjdbDatabase::addDatabase(DBURL, MODE);
            QEjdbDatabase::database().createCollection(COLL);

            for (int i = 1; i < 11; i++) {
                QBsonObject obj;
                obj.append("name", QLatin1String("test") + QString::number(i));
                QEjdbDatabase::database().save(COLL, obj);
            }
        }

        static void TearDownTestCase()
        {
            QEjdbDatabase::database().removeCollection(COLL);
            QEjdbDatabase::removeDatabase();
        }

};


TEST_F(QEjdbResultTest, TestCount)
{
    QEjdbResult result = QEjdbDatabase::database().loadAll(COLL);
    EXPECT_EQ(10, result.count());
}
TEST_F(QEjdbResultTest, TestValue)
{
    QEjdbResult result = QEjdbDatabase::database().loadAll(COLL);
    QList<QBsonObject> values = result.values();
    EXPECT_EQ(10, values.size());
}
TEST_F(QEjdbResultTest, TestFirst)
{
    QEjdbResult result = QEjdbDatabase::database().loadAll(COLL);
    // second time it will tranformed in qbsonobject
    EXPECT_EQ("test1", result.first().value("name"));
    // second time it will not tranformed
    EXPECT_EQ("test1", result.first().value("name"));
}
TEST_F(QEjdbResultTest, TestAssignment)
{
    QEjdbResult result = QEjdbDatabase::database().loadAll(COLL);
    QEjdbResult result2 = checkAssignment(result);
    EXPECT_EQ(10, result.count());
}
TEST_F(QEjdbResultTest, TestReadWriteDataStream)
{
    QByteArray binary;
    QDataStream dw(&binary, QIODevice::WriteOnly);
    QDataStream dr(&binary, QIODevice::ReadOnly);

    QEjdbResult result = QEjdbDatabase::database().loadAll(COLL);
    QEjdbResult result2;

    dw << result;
    dr >> result2;

     EXPECT_EQ(10, result2.count());
}
