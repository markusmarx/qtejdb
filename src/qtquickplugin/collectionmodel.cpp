#include "collectionmodel.h"
#include "qejdbclient.h"
#include "itemmodel/qejdbcollectionsync.h"
#include <QDebug>

CollectionModel::CollectionModel(QObject *parent)
    :BaseModel(parent)
{

}

CollectionModel::~CollectionModel()
{

}

void CollectionModel::classBegin()
{

}

bool CollectionModel::checkProperties()
{
    bool error = false;

    if (m_collection.isEmpty()) {
        error = true;
    }
    if (m_client == 0) {
        error = true;
    }
    return !error;
}

void CollectionModel::tryInit()
{
    if (checkProperties()) {
        m_client->registerModel(this);
    }
}

void CollectionModel::componentComplete()
{

}


QJSValue CollectionModel::query() const
{
    return m_query;
}

QJSValue CollectionModel::hints() const
{
    return m_hints;
}

void CollectionModel::setQuery(QJSValue query)
{
    if (m_query.equals(query))
        return;

    m_query = query;
    emit queryChanged(m_client->convert(query));

}

void CollectionModel::setHints(QJSValue hints)
{
    if (m_hints.equals(hints))
        return;

    m_hints = hints;
    emit hintsChanged(m_client->convert(hints));
    emit fetch();
}

void CollectionModel::connected()
{
    if (m_client->isConnected() && checkProperties()) {
        QEjdbDatabase db = QEjdbDatabase::database(m_client->connectionName());
        QBsonObject query = m_client->convert(m_query);
        QBsonObject hints = m_client->convert(m_hints);
        QEjdbCollectionSync *colSync = new QEjdbCollectionSync(db, m_collection, query, hints, this);

        QObject::connect(this, &CollectionModel::queryChanged,
                         colSync, &QEjdbCollectionSync::setQuery
                         );
        QObject::connect(this, &CollectionModel::hintsChanged,
                         colSync, &QEjdbCollectionSync::setHints
                         );
        QObject::connect(this, &CollectionModel::collectionChanged,
                         colSync, &QEjdbCollectionSync::setCollection
                         );
        QObject::connect(this, &CollectionModel::fetch,
                         colSync, &QEjdbCollectionSync::fetch
                         );

        QEjdbItemModel::setSync(colSync);
        return;
    }
}

void CollectionModel::disconnected()
{

}
