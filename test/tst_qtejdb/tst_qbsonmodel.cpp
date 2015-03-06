#include "tst_qbsonmodel.h"
#include "qbsonitemmodel_p.h"
#include <QTest>

#define TEST_COUNT 10

Tst_QBsonModel::Tst_QBsonModel(QObject *parent):QObject(parent)
{

}

Tst_QBsonModel::~Tst_QBsonModel()
{

}

/**
 * @brief Tst_QBsonModel::itemRemoved slot to test signal itemRemoved.
 */
void Tst_QBsonModel::itemRemoved(QBsonObject removedObject)
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
void Tst_QBsonModel::itemUpdated(int row)
{
    m_emitItemUpdated = row;
}

void Tst_QBsonModel::initTestCase()
{

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

    qBsonModel.set(createTestBsonObjectList(TEST_COUNT, true));

    QCOMPARE(qBsonModel.roles().count(), 11);
    QCOMPARE(qBsonModel.count(), TEST_COUNT);

    testBsonObject(qBsonModel.row(0));
    QString id = qBsonModel.row(5).value("_id").toString();
    QString id2 = qBsonModel.oid(id).value("_id").toString();
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

    QBsonObject updatedObject = createTestBsonObject(11);
    qBsonModel.update(updatedObject, 5);
    testBsonObjectMarker(qBsonModel.row(5), 11);
    QCOMPARE(m_emitItemUpdated, 5);
}

QBsonObject Tst_QBsonModel::createTestBsonObject(int marker)
{
    QBsonObject bsonObject = Tst_Base::createTestBsonObject(true);
    bsonObject.insert("marker", QBsonValue(marker));
    return bsonObject;
}

void Tst_QBsonModel::testBsonObjectMarker(QBsonObject bsonObject, int marker)
{
    QCOMPARE(bsonObject.value("marker").toInt(), marker);
}

