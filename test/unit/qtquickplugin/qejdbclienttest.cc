#include "gtest/gtest.h"
#include "qtquickplugin/qejdbclient.h"
#include "../helper.h"

#define MODE QEJDB::CREATE | QEJDB::WRITE | \
    QEJDB::SYNC | QEJDB::LOCK_NB | QEJDB::TRUNCATE

#define DBURL "file:test_qejdbclient.db"
#define COLL "testCollection"
#define COLL2 "testCollection1"
#define PROP "arrayProperty"
#define CONN_NAME "connection"

class QEjdbClientTest : public ::testing::Test
{
    protected:

        static void SetUpTestCase()
        {
            QEjdbDatabase::addDatabase(DBURL, MODE, CONN_NAME);
            QEjdbDatabase::database(CONN_NAME).createCollection(COLL);
            QEjdbDatabase::database(CONN_NAME).createCollection(COLL2);

            QBsonArray array;
            for (int i = 1; i < 11; i++) {
                QBsonObject obj = createBsonObject(false, i);
                QEjdbDatabase::database(CONN_NAME).save(COLL, obj);
                array.append(obj.oid());
            }

            QBsonObject object;
            object.insert(PROP, array);
            QEjdbDatabase::database(CONN_NAME).save(COLL2, object);

        }

        static void TearDownTestCase()
        {
            QEjdbDatabase::database(CONN_NAME).removeCollection(COLL);
            QEjdbDatabase::database(CONN_NAME).removeCollection(COLL2);
            QEjdbDatabase::removeDatabase(CONN_NAME);
        }

public:
        static QString oid;

};

QString QEjdbClientTest::oid;

TEST_F(QEjdbClientTest, TestConnectionWithExistdb)
{
    QEjdbClient *client = new QEjdbClient;
    client->setConnectionName(CONN_NAME);
    client->componentComplete();
    client->connect();
    EXPECT_TRUE(QEjdbDatabase::database(CONN_NAME).isOpen());
    delete client;
}
TEST_F(QEjdbClientTest, TestConnectionWithNoExistDb)
{
    QEjdbClient *client = new QEjdbClient;
    client->setUri("file:test_qejdbclient_notexist.db");
    client->componentComplete();
    client->connect();
    EXPECT_TRUE(QEjdbDatabase::database(client->connectionName()).isOpen());
    client->disconnect();
    delete client;
}
TEST_F(QEjdbClientTest, TestSave)
{
    QEjdbClient *client = new QEjdbClient;
    client->setConnectionName(CONN_NAME);
    client->setUri(DBURL);
    client->componentComplete();
    client->connect();

    QJSEngine *jsEngine = new QJSEngine();
    QJSValue obj = jsEngine->newObject();
    obj.setProperty("name", "test");
    obj = client->save(COLL, obj);
    EXPECT_TRUE(obj.hasProperty("_id"));
    QEjdbClientTest::oid = obj.property("_id").toString();
    delete client;
}
TEST_F(QEjdbClientTest, TestLoad)
{
    QEjdbClient *client = new QEjdbClient;
    client->setConnectionName(CONN_NAME);
    client->setUri(DBURL);
    client->componentComplete();
    client->connect();

    QJSValue obj = client->load(COLL, QEjdbClientTest::oid);
    EXPECT_EQ("test", obj.property("name").toString());
    delete client;
}
TEST_F(QEjdbClientTest, TestRemove)
{
    QEjdbClient *client = new QEjdbClient;
    client->setConnectionName(CONN_NAME);
    client->setUri(DBURL);
    client->componentComplete();
    client->connect();

    client->remove(COLL, QEjdbClientTest::oid);
    QJSValue obj = client->load(COLL, QEjdbClientTest::oid);
    EXPECT_TRUE(obj.isNull());
    delete client;
}
