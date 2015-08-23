#include "gtest/gtest.h"
#include "qtejdb/qejdbquery.h"
#include "qtejdb/qejdbdatabase.h"
#include "qtejdb/qejdbresult.h"

#define MODE QEJDB::CREATE | QEJDB::WRITE | \
    QEJDB::SYNC | QEJDB::LOCK_NB | QEJDB::TRUNCATE

#define DBURL "file:test_query.db"
#define COLL "testCollection"

QEjdbQuery testAssignment(const QEjdbQuery &query)
{
    QEjdbQuery query2 = query;
    return query2;
}

class QEjdbQueryTest : public ::testing::Test
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
            QEjdbDatabase::database().dropCollection(COLL);
            QEjdbDatabase::removeDatabase();
        }

};

TEST_F(QEjdbQueryTest, TestCollectionName)
{
    QEjdbQuery query(COLL, QEjdbDatabase::database());
    EXPECT_EQ(COLL, query.collectionName());
}
TEST_F(QEjdbQueryTest, TestQuery)
{
    QEjdbQuery query(COLL, QEjdbDatabase::database());
    QEjdbResult result = query.exec(QBsonObject("name", "test1"));
    EXPECT_EQ(1, result.count());
    EXPECT_EQ("test1", result.first().value("name"));
}
TEST_F(QEjdbQueryTest, TestAssign)
{
    QEjdbQuery query(COLL, QEjdbDatabase::database());
    QEjdbQuery query2 = testAssignment(query);
    EXPECT_EQ(COLL, query2.collectionName());
}

