#include "tst_qbsonmodel.h"
#include "qbsonitemmodel_p.h"
#include <QTest>

#define TEST_COUNT 10

Tst_QBsonModel::Tst_QBsonModel(QString url, QObject *parent)
    :QObject(parent), m_url(url), m_emitReset(false)
{

}

Tst_QBsonModel::~Tst_QBsonModel()
{

}

/**
 * @brief Tst_QBsonModel::itemRemoved slot to test signal itemRemoved.
 */
void Tst_QBsonModel::itemRemoved(int row, QBsonObject removedObject)
{
    m_emitItemRemoved = removedObject;
}

/**
 * @brief Tst_QBsonModel::itemInserted slot to test signal itemInserted.
 */
void Tst_QBsonModel::itemInserted(int row)
{
    m_emitItemInserted = row;
}

/**
 * @brief Tst_QBsonModel::itemMoved slot to test signal itemMoved.
 */
void Tst_QBsonModel::itemMoved(int sourceRow, int destinationRow)
{
    m_emitItemMovedSourceRow = sourceRow;
    m_emitItemMovedDestinationRow = destinationRow;
}

/**
 * @brief Tst_QBsonModel::itemUpdated slot to test signal itemUpdated.
 */
void Tst_QBsonModel::itemUpdated(QString property, QVariant value, int row)
{
    m_emitItemUpdated = row;
    m_emitItemUpdatedProperty = property;
    m_emitItemUpdatedValue = value;
}

void Tst_QBsonModel::reset()
{
    m_emitReset = true;
}

void Tst_QBsonModel::initTestCase()
{
    QEjdbDatabase::removeDatabaseFiles(".", "test_db");
    QEjdbDatabase::addDatabase(m_url).open();
}

QBsonObject Tst_QBsonModel::createTestBsonObject(int marker, bool withId)
{
    QBsonObject bsonObject = Tst_Base::createTestBsonObject(withId);
    bsonObject.insert("marker", QBsonValue(marker));
    return bsonObject;
}

void Tst_QBsonModel::testBsonObjectMarker(QBsonObject bsonObject, int marker)
{
    QCOMPARE(bsonObject.value("marker").toInt(), marker);
}

/**
 * @brief Tst_QBsonModel::tst_QBsonModelSimple test setting a list and retrieve
 * data.
 */
void Tst_QBsonModel::tst_QBsonModelSimple()
{

    QBsonItemModel qBsonModel;
    QObject::connect(&qBsonModel, &QBsonItemModel::itemInserted,
                     this, &Tst_QBsonModel::itemInserted);
    QObject::connect(&qBsonModel, &QBsonItemModel::itemRemoved,
                     this, &Tst_QBsonModel::itemRemoved);
    QObject::connect(&qBsonModel, &QBsonItemModel::itemMoved,
                     this, &Tst_QBsonModel::itemMoved);
    QObject::connect(&qBsonModel, &QBsonItemModel::itemUpdated,
                     this, &Tst_QBsonModel::itemUpdated);
    QObject::connect(&qBsonModel, &QBsonItemModel::reset,
                     this, &Tst_QBsonModel::reset);


    qBsonModel.insert(Tst_Base::createTestBsonObject(true), 0);
    QCOMPARE(qBsonModel.roles().count(), 10);
    QCOMPARE(qBsonModel.count(), 1);
    QCOMPARE(m_emitReset, true);
    m_emitReset = false;

    qBsonModel.set(createTestBsonObjectList(TEST_COUNT, true));
    QCOMPARE(m_emitReset, true);

    QCOMPARE(qBsonModel.roles().count(), 10);
    QCOMPARE(qBsonModel.count(), TEST_COUNT);

    testBsonObject(qBsonModel.row(0));
    QString id = qBsonModel.row(5).oid().toString();
    QString id2 = qBsonModel.oid(id).oid().toString();
    QCOMPARE(id, id2);

    QBsonObject obj = createTestBsonObject(99);
    qBsonModel.append(obj);
    QCOMPARE(m_emitItemInserted, qBsonModel.count()-1);
    obj = createTestBsonObject(1);
    qBsonModel.insert(obj, -1);
    qBsonModel.insert(obj, qBsonModel.count() + 1);
    qBsonModel.insert(obj, 0);
    QCOMPARE(m_emitItemInserted, 0);
    obj = createTestBsonObject(5);
    qBsonModel.insert(obj, 5);
    QCOMPARE(m_emitItemInserted, 5);

    testBsonObjectMarker(qBsonModel.row(5), 5);
    testBsonObjectMarker(qBsonModel.row(0), 1);
    testBsonObjectMarker(qBsonModel.row(qBsonModel.count() - 1), 99);

    qBsonModel.move(-1, qBsonModel.count() + 1);
    qBsonModel.move(0, qBsonModel.count());
    QCOMPARE(m_emitItemMovedSourceRow, 0);
    QCOMPARE(m_emitItemMovedDestinationRow, qBsonModel.count() - 1);
    testBsonObjectMarker(qBsonModel.row(qBsonModel.count() - 1), 1);

    qBsonModel.move(qBsonModel.count()-1, 0);
    QCOMPARE(m_emitItemMovedSourceRow, qBsonModel.count()-1);
    QCOMPARE(m_emitItemMovedDestinationRow, 0);
    testBsonObjectMarker(qBsonModel.row(0), 1);

    qBsonModel.move(0, 5);
    QCOMPARE(m_emitItemMovedSourceRow, 0);
    QCOMPARE(m_emitItemMovedDestinationRow, 4);
    testBsonObjectMarker(qBsonModel.row(4), 1);

    qBsonModel.remove(3);
    testBsonObject(m_emitItemRemoved);
    testBsonObjectMarker(qBsonModel.row(3), 1);

    QCOMPARE(qBsonModel.count(), TEST_COUNT+2);

    qBsonModel.update("marker",11, 5);
    testBsonObjectMarker(qBsonModel.row(5), 11);
    QCOMPARE(m_emitItemUpdated, 5);
    QCOMPARE(m_emitItemUpdatedProperty, QString("marker"));
    QCOMPARE(m_emitItemUpdatedValue, QVariant(11));

    QHash<int, QByteArray> roles = qBsonModel.roles();
    int testRole = roles.key(QString("string").toLatin1());
    QCOMPARE(qBsonModel.data(5, testRole).toString(), QString("test"));
    qBsonModel.setData(5, testRole, QString("testtest"));
    QCOMPARE(qBsonModel.data(5, testRole).toString(), QString("testtest"));

}

/**
 * @brief Tst_QBsonModel::tst_QEjdbCollectionSync
 */
void Tst_QBsonModel::tst_QEjdbCollectionSync()
{
    // open database and create collection
    QEjdbDatabase db = QEjdbDatabase::database();
    db.createCollection("testCollection4");
    QEjdbCollectionSync *sync
            = new QEjdbCollectionSync(QEjdbDatabase::database());
    sync->setCollection("testCollection4");

    // fetch model
    sync->fetch();
    // create object in model
    sync->model()->append(createTestBsonObject(11));
    QCOMPARE(sync->model()->count(), 1);

    // load objects from db and test
    QEjdbResult result = db.query("testCollection4", QBsonObject());
    QCOMPARE(result.count(), 1);
    QBsonObject bsonObject = result.first();
    testBsonObjectMarker(bsonObject, 11);

    // change object in model
    sync->model()->update("marker", 12, 0);
    result = db.query("testCollection4", QBsonObject());
    QCOMPARE(result.count(), 1);
    bsonObject = result.first();
    testBsonObjectMarker(bsonObject, 12);

    // add new object
    sync->model()->append(createTestBsonObject(13));
    result = db.query("testCollection4", QBsonObject());
    QCOMPARE(result.count(), 2);
    QList<QBsonObject> objList = result.values();
    testBsonObjectMarker(objList.at(0), 12);
    testBsonObjectMarker(objList.at(1), 13);

    // test concurrent change
    // change the object in db
    bsonObject.insert("test_44", "test_44");
    db.save("testCollection4", bsonObject);
    // change the object in model
    sync->model()->update("marker", 14, 0);
    // test if the change in the db was not overwritten
    bsonObject = db.load("testCollection4", bsonObject.oid());
    testBsonObjectMarker(bsonObject, 14);
    QCOMPARE(bsonObject.value("test_44").toString(), QString("test_44"));

    // fetch
    sync->fetch();
    QCOMPARE(sync->model()->count(), 2);

    delete sync;
}

#define COLLECTION_OBJECT "testcollection5"
#define COLLECTION_ARRAY  "testcollection6"

/**
 * @brief tests array property sync where the parent object and his sub objects
 * are connected with joins. Means that only id are stored in parent object.
 */
void Tst_QBsonModel::tst_QEjdbArrayPropertySyncWithJoin()
{
    // open database and create collection
    QEjdbDatabase db = QEjdbDatabase::database();
    db.createCollection(COLLECTION_OBJECT);
    db.createCollection(COLLECTION_ARRAY);
    QEjdbArrayPropertySync *sync
            = new QEjdbArrayPropertySync(QEjdbDatabase::database());

    QBsonObject bsonObject = createTestBsonObject(11);
    sync->setBsonObject(bsonObject, "testArray");
    sync->setCollection(COLLECTION_OBJECT);
    sync->setPropertyCollection(COLLECTION_ARRAY);

    sync->fetch();
    // create subproperties
    QBsonObject pos1Bson = createTestBsonObject(1);
    QBsonObject pos2Bson = createTestBsonObject(2);
    sync->model()->append(pos1Bson);
    sync->model()->append(pos2Bson);

    QEjdbResult result = db.query(COLLECTION_ARRAY, QBsonObject());
    QCOMPARE(result.count(), 2);

    result = db.query(COLLECTION_OBJECT, QBsonObject());
    QCOMPARE(result.count(), 1);
    bsonObject = result.first();
    QBsonArray array = result.first().value("testArray").toArray();
    QCOMPARE(array.size(), 2);
    QCOMPARE(array.value(0).toString(), pos1Bson.oid().toString());
    QCOMPARE(array.value(1).toString(), pos2Bson.oid().toString());

    sync->fetch();

    QCOMPARE(sync->model()->row(0).oid(), pos1Bson.oid());
    QCOMPARE(sync->model()->row(1).oid(), pos2Bson.oid());

    sync->model()->move(1, 0);
    QCOMPARE(sync->model()->row(1).oid(), pos1Bson.oid());
    QCOMPARE(sync->model()->row(0).oid(), pos2Bson.oid());

    sync->fetch();
    QCOMPARE(sync->model()->count(), 2);
    QCOMPARE(sync->model()->row(1).oid(), pos1Bson.oid());
    QCOMPARE(sync->model()->row(0).oid(), pos2Bson.oid());

    sync->model()->move(0, 1);
    sync->model()->update("mytest", "mytest", 1);
    QBsonObject pos0Bson = createTestBsonObject(25);
    QBsonObject pos3Bson = createTestBsonObject(26);
    sync->model()->insert(pos0Bson, 0);
    sync->model()->insert(pos3Bson, 3);
    sync->fetch();
    QCOMPARE(sync->model()->count(), 4);
    QCOMPARE(sync->model()->row(2).oid(), pos1Bson.oid());
    QCOMPARE(sync->model()->row(1).oid(), pos2Bson.oid());
    QCOMPARE(sync->model()->row(2).value("mytest").toString(),
             QString("mytest"));

    db.removeCollection(COLLECTION_OBJECT);
    db.removeCollection(COLLECTION_ARRAY);
}

/**
 * @brief tests array property sync where sub objects are stored direct in his
 * parent.
 */
void Tst_QBsonModel::tst_QEjdbArrayPropertySyncWithoutJoin()
{
    // open database and create collection
    QEjdbDatabase db = QEjdbDatabase::database();
    db.createCollection(COLLECTION_OBJECT);

    QEjdbArrayPropertySync *sync
            = new QEjdbArrayPropertySync(QEjdbDatabase::database());

    QBsonObject bsonObject = createTestBsonObject(11);
    sync->setBsonObject(bsonObject, "testArray");
    sync->setCollection(COLLECTION_OBJECT);

    sync->fetch();
    // create subproperties
    QBsonObject pos1Bson = createTestBsonObject(1, false);
    QBsonObject pos2Bson = createTestBsonObject(2, false);
    sync->model()->append(pos1Bson);
    sync->model()->append(pos2Bson);

    QEjdbResult result = db.query(COLLECTION_OBJECT, QBsonObject());
    QCOMPARE(result.count(), 1);
    bsonObject = result.first();
    QBsonArray array = result.first().value("testArray").toArray();
    QCOMPARE(array.size(), 2);
    testBsonObjectMarker(array.value(0).toObject(), 1);
    testBsonObjectMarker(array.value(1).toObject(), 2);

    sync->fetch();

    QCOMPARE(sync->model()->row(0).oid(), pos1Bson.oid());
    QCOMPARE(sync->model()->row(1).oid(), pos2Bson.oid());

    sync->model()->move(1, 0);
    QCOMPARE(sync->model()->row(1).oid(), pos1Bson.oid());
    QCOMPARE(sync->model()->row(0).oid(), pos2Bson.oid());

    sync->fetch();
    QCOMPARE(sync->model()->count(), 2);
    QCOMPARE(sync->model()->row(1).oid(), pos1Bson.oid());
    QCOMPARE(sync->model()->row(0).oid(), pos2Bson.oid());

    sync->model()->move(0, 1);
    sync->model()->update("mytest", "mytest", 1);
    QBsonObject pos0Bson = createTestBsonObject(25);
    QBsonObject pos3Bson = createTestBsonObject(26);
    sync->model()->insert(pos0Bson, 0);
    sync->model()->insert(pos3Bson, 3);
    sync->fetch();
    QCOMPARE(sync->model()->count(), 4);
    QCOMPARE(sync->model()->row(2).oid(), pos1Bson.oid());
    QCOMPARE(sync->model()->row(1).oid(), pos2Bson.oid());
    QCOMPARE(sync->model()->row(2).value("mytest").toString(),
             QString("mytest"));


}

