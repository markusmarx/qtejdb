#include "qbsonitemmodel_p.h"

/**
 * @internal
 * @class QBsonItemModel
 * @brief QBsonItemModel QBsonItemModel is a internal storage for bson objects.
 * Notifies about changes. Used in QEjdbItemModel
 *
 * @fn itemMoved()
 * @fn item
 *
 * @see QEjdbItemModel.
 */

QBsonItemModel::QBsonItemModel(QObject *parent)
    : QObject(parent)
{

}

/**
 * @internal
 * @brief QBsonItemModel::~QBsonItemModel destruct instance. Does nothing.
 */
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
 * @brief QBsonItemModel::update Update a row object. Emit a itemUpdated signal.
 */
void QBsonItemModel::update(QBsonObject &bsonObject, const uint &row)
{
    if (isValidRow(row) && bsonObject.contains("_id")) {
        internalRemove(row);
        internalInsert(bsonObject, row);
        emit itemUpdated(row);
    }
}

/**
 * @brief QBsonItemModel::append Append the QBsonObject to model. emit a itemInsert
 * signal.
 */
void QBsonItemModel::append(QBsonObject &bsonObject)
{
    int row = count();
    internalInsert(bsonObject, row);
    emit itemInserted(row);
}

/**
 * @brief QBsonItemModel::move Moves a row within model. This function assumes
 * that both sourceRow and destinationRow are at least 0 but less that count().
 * In case of failure no row are moved.
 *
 */
void QBsonItemModel::move(int sourceRow, int destinationRow)
{
    if (sourceRow == destinationRow)
        return;

    if (sourceRow < destinationRow) {
        destinationRow--;
    }
    if (isValidRow(sourceRow)
            && destinationRow >= 0 && destinationRow <= count()) {
        m_bsonList.move(sourceRow, destinationRow);
        emit itemMoved(sourceRow, destinationRow);
    }

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
 * @brief QBsonItemModel::buildRoles take a bsonobject and build roles from
 * names list.
 */
void QBsonItemModel::buildRoles()
{
    int i = Qt::UserRole + 100;
    m_roles.clear();
    if (m_bsonObjects.count() == 0) return;
    foreach (QString name, m_bsonObjects.values().first().names()) {
        m_roles.insert(i, name.toLatin1());
        i++;
    }
}

/**
 * @brief QBsonItemModel::roles Returns roles map. The roles map is build with
 * QBsonItemModel::buildRoles
 */
QHash<int, QByteArray> QBsonItemModel::roles()
{
    return m_roles;
}


/**
 * @brief QBsonItemModel::getByRow Returns bson stored in row.
 *
 * @param row row value
 *
 * @return bson stored in row
 */
QBsonObject QBsonItemModel::row(int row)
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
QBsonObject QBsonItemModel::oid(QBsonItemModel::QBsonId bsonId)
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
    for (it = bsonList.begin(); it != bsonList.end(); it++) {
        internalInsert(*it, i++);
    }
    buildRoles();
    emit reset();
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

