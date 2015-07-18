#include "qbsonitemmodel_p.h"
/**
 * @class QBsonItemModel
 * @brief QBsonItemModel is a internal storage for bson objects.
 * Notifies about changes.
 * The diagramm shows the sequence how to fetched data from db.
 * @startuml{sequence_qejdbitemmodel_fetching_from_db} "Fetching documents from db"
 *   User --> QEjdbItemModel: <<create>>
 *   activate QEjdbItemModel
 *   QEjdbItemModel --> QEjdb..Sync: <<create>>
 *   QEjdbItemModel --> QEjdb..Sync: fetch
 *   QEjdb..Sync --> QBsonItemModel: set
 *   QEjdbItemModel --> QEjdbItemModel: <<reset model>>
 *   QEjdbItemModel --> QBsonItemModel: row
 *   User --> QEjdbItemModel: <<delete>>
 *   deactivate QEjdbItemModel
 * @enduml
 *
 * @see QEjdbItemModel
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
void QBsonItemModel::insert(const QBsonObject &bsonObject, const uint &row)
{
    if (row <= count()) {
        internalInsert(bsonObject, row);
        emit itemInserted(row);
        if (count() == 1) {
            buildRoles();
        }
    }

}

/**
 * @brief QBsonItemModel::update Update a row with property and value.
 * Emit a itemUpdated signal.
 */
void QBsonItemModel::update(QString property, QVariant value, const uint &r)
{

    if (isValidRow(r)) {
        QBsonObject obj = row(r).insert(property, QBsonValue::fromVariant(value));
        emit itemUpdated(property, value, r);
    }
}

/**
 * @brief QBsonItemModel::append Append the QBsonObject to model. emit a itemInsert
 * signal.
 */
void QBsonItemModel::append(QBsonObject bsonObject)
{
    int row = count();
    internalInsert(bsonObject, row);
    emit itemInserted(row);
    if (row == 0) {
        buildRoles();
    }
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
        emit itemRemoved(row, bsonObject);
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
    emit reset();
}

/**
 * @brief QBsonItemModel::clear clear all data from model.
 *
 * @return void
 */
void QBsonItemModel::clear()
{
    internalClear();
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
 * @brief Returns value by row and role.
 */
QBsonValue QBsonItemModel::data(int row, int role)
{
    if (isValidRow(row)) {
        return internalGet(row).value(roleAsKey(role));
    }
    return QBsonValue();
}

/**
 * @brief Set value by row and role.
 */
void QBsonItemModel::setData(int row, int role, const QBsonValue &value)
{
    QString key = roleAsKey(role);
    if (isValidRow(row) && !key.isEmpty()) {
        internalGet(row).insert(roleAsKey(role), value);
        emit itemUpdated(key, value.toVariant(), row);
    }
}

/**
 * @brief QBsonItemModel::getById Returns bson by bsonid
 * @param bsonId
 * @return bson from id
 */
QBsonObject QBsonItemModel::byOid(QBsonItemModel::QBsonId bsonId)
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
    internalClear();
    QList<QBsonObject>::iterator it;
    int i = 0;
    for (it = bsonList.begin(); it != bsonList.end(); it++) {
        internalInsert(*it, i++);

    }
    buildRoles();
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

/**
 * @brief Returns the value that stored under role in m_roles.
 */
QString QBsonItemModel::roleAsKey(int role)
{
    if (m_roles.contains(role)) {
        return QString(m_roles[role]);
    }
    return QString();
}
