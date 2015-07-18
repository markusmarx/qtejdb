#include "qejdbcollectionsync.h"

/**
 * @class QEjdbCollectionSync manages bson document synchronisation with a
 * collection. Synchronize a qbsonmodel with a collection in database. Create,
 * update, remove bsonobjects. A row insert append the object into the database collection.
 */

/**
 * @brief QEjdbCollectionSync::QEjdbCollectionSync initialize a instance with
 * a database and an underlying model that caches the data.
 */
QEjdbCollectionSync::QEjdbCollectionSync(QEjdbDatabase db, QObject *parent)
    : m_db(db), QEjdbAbstractSync(parent)
{
}

/**
 * @brief QEjdbCollectionSync::~QEjdbCollectionSync deletes bsonModel
 */
QEjdbCollectionSync::~QEjdbCollectionSync()
{
}

/**
 * @brief QEjdbCollectionSync::query Returns the query object. A query is a bson
 * structure to fetch and filter a collection of bson objects from database.
 * @see QEjdbCollectionSync::setQuery()
 */
QBsonObject QEjdbCollectionSync::query() const
{
    return m_query;
}

/**
 * @brief QEjdbCollectionSync::hints returns the hints. Hints are additional
 * options to control data fetching.
 */
QBsonObject QEjdbCollectionSync::hints() const
{
    return m_hints;
}

/**
 * @brief QEjdbCollectionSync::collection returns the collection. A collection
 * is a container for bson objects. So you have to define the collection.
 */
QString QEjdbCollectionSync::collection() const
{
    return m_collection;
}

/**
 * @brief QEjdbCollectionSync::fetch get data from collection with query and
 * hints.
 */
void QEjdbCollectionSync::fetch()
{
    if (isDbValid()) {
        QEjdbResult result = m_db.query(m_collection, m_query, m_hints);
        m_qBsonItemModel->set(result.values());
    }
}

/**
 * @brief QEjdbCollectionSync::setQuery set a new query value.
 * @see QEjdbCollectionSync::query()
 */
void QEjdbCollectionSync::setQuery(QBsonObject query)
{
    m_query = query;
}

/**
 * @brief QEjdbCollectionSync::setHints set new hints.
 * @see QEjdbCollectionSync::hints()
 */
void QEjdbCollectionSync::setHints(QBsonObject hints)
{
    m_hints = hints;
}

/**
 * @brief QEjdbCollectionSync::setCollection set a new collection.
 */
void QEjdbCollectionSync::setCollection(QString collection)
{
    m_collection = collection;
}

/**
 * @brief QEjdbCollectionSync::itemRemoved remove item from db.
 */
void QEjdbCollectionSync::itemRemoved(int row, QBsonObject removedObject)
{
    Q_UNUSED(row)
    if (isDbValid()) {
        m_db.remove(m_collection, removedObject);
    }
}

/**
 * @brief QEjdbCollectionSync::itemInserted update the bsonobject at given row.
 */
void QEjdbCollectionSync::itemInserted(int row)
{
    if (isDbValid()) {
        QBsonObject bsonObject = m_qBsonItemModel->row(row);
        m_db.save(m_collection, bsonObject);
    }
}

/**
 * @brief QEjdbCollectionSync::itemUpdated load a bson from db change it and
 * saves back.
 */
void QEjdbCollectionSync::itemUpdated(QString property, QVariant value, int row)
{
    if (isDbValid()) {
        QBsonObject obj = m_qBsonItemModel->row(row);
        QBsonObject loadedObj = m_db.load(m_collection, obj.oid());
        loadedObj.insert(property, QBsonValue::fromVariant(value));
        m_db.save(m_collection, loadedObj);
    }

}

void QEjdbCollectionSync::itemMoved(int sourceRow, int destinationRow)
{
    Q_UNUSED(sourceRow); Q_UNUSED(destinationRow);
}

