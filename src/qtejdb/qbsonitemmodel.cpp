#include "qbsonitemmodel_p.h"

/**
 * @class QBsonItemModel
 * @brief QBsonItemModel QBsonItemModel is a internal storage for bson objects
 * used @see QEjdbItemModel.
 */

QBsonItemModel::QBsonItemModel(QObject *parent) : QObject(parent)
{

}

QBsonItemModel::~QBsonItemModel()
{

}

/**
 * @brief QBsonItemModel::insert insert the bson on the given row. Row starts
 * with 0. If row greater than count the bson is not inserted.
 *
 * @param bsonObject bsonobject
 * @param row row
 */
void QBsonItemModel::insert(QBsonObject &bsonObject, const uint &row)
{
    if (row <= count()) {
        internalInsert(bsonObject, row);
        emit itemInserted(row);
    }

}

/**
 * @brief QBsonItemModel::append
 * @param bsonObject
 */
void QBsonItemModel::append(QBsonObject &bsonObject)
{
    int row = count();
    internalInsert(bsonObject, row++);
    emit itemInserted(row);
}

/**
 * @brief QBsonItemModel::remove Removes item on the given row.
 *
 * @param row row
 */
void QBsonItemModel::remove(int row)
{
    if (isValidRow(row)) {
        QBsonObject bsonObject = internalRemove(row);
        emit itemRemoved(bsonObject);
    }
}

/**
 * @brief QBsonItemModel::getByRow Returns bson stored in row.
 *
 * @param row row value
 *
 * @return bson stored in row
 */
QBsonObject QBsonItemModel::getByRow(int row)
{
    if (isValidRow(row)) {
        return internalGet(row);
    }
    return QBsonObject();
}

/**
 * @brief QBsonItemModel::getById Returns bson by bsonid
 * @param bsonId
 * @return bson from id
 */
QBsonObject QBsonItemModel::getById(QBsonItemModel::QBsonId bsonId)
{
    return internalGet(bsonId);
}

/**
 * @brief QBsonItemModel::set set a bson list. Clear all contained bsondata.
 * emit a reset signal
 *
 * @param bsonList new bsonlist to store
 *
 * @return void
 */
void QBsonItemModel::set(QList<QBsonObject> bsonList)
{
    QList<QBsonObject>::iterator it;
    int i = 0;
    for (it = bsonList.begin(); it != bsonList.end(); i++) {
        internalInsert(*it, i++);
    }
}

/**
 * @brief QBsonItemModel::count Returns item count in model.
 *
 * @return item count
 */
int QBsonItemModel::count()
{
    return m_bsonList.count();
}

