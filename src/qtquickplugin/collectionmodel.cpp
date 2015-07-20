#include "collectionmodel.h"
#include "qejdbclient.h"
#include "itemmodel/qejdbcollectionsync.h"
#include <QDebug>

CollectionModel::CollectionModel(QObject *parent)
    :QEjdbItemModel(parent)
{

}

CollectionModel::~CollectionModel()
{

}

void CollectionModel::classBegin()
{

}

void CollectionModel::componentComplete()
{
    if (m_client != 0) {
        qWarning() << "the property \"client\" have to set in CollectionModel";
    }

    if (m_collection.isEmpty()) {
        qWarning() << "the property \"collection\" have to set in CollectionModel";
    }

    QEjdbDatabase db = QEjdbDatabase::database(m_client->connectionName());
    QBsonObject query = m_client->convert(m_query);
    QBsonObject hints = m_client->convert(m_hints);
    setSync(new QEjdbCollectionSync(db, m_collection, query, hints, this));
}

/**
 * @brief Returns the client that connects to the db.
 * @return
 */
QEjdbClient *CollectionModel::client() const
{
    return m_client;
}

QJSValue CollectionModel::query() const
{
    return m_query;
}

QJSValue CollectionModel::hints() const
{
    return m_hints;
}

QString CollectionModel::collection() const
{
    return m_collection;
}

/**
 * @brief Sets a new client.
 */
void CollectionModel::setClient(QEjdbClient *client)
{
    if (m_client != client) {
        m_client = client;
    }
    emit clientChanged();
}

void CollectionModel::setQuery(QJSValue query)
{
    if (m_query.equals(query))
        return;

    m_query = query;
    emit queryChanged(query);
}

void CollectionModel::setHints(QJSValue hints)
{
    if (m_hints.equals(hints))
        return;

    m_hints = hints;
    emit hintsChanged(hints);
}

void CollectionModel::setCollection(QString collection)
{
    if (m_collection == collection)
        return;

    m_collection = collection;
    emit collectionChanged(collection);
}

void CollectionModel::insert(QJSValue value, int row)
{
    QEjdbItemModel::insert(m_client->convert(value), row);
}
