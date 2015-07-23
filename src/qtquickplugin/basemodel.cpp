#include "basemodel.h"

BaseModel::BaseModel(QObject *parent)
    :QEjdbItemModel(parent), m_client(0)
{

}

QEjdbClient *BaseModel::client() const
{
    return m_client;
}

QString BaseModel::collection() const
{
    return m_collection;
}

void BaseModel::setClient(QEjdbClient *client)
{
    if (m_client == client)
        return;

    m_client = client;
    emit clientChanged(client);
    tryInit();
    emit fetch();
}

void BaseModel::setCollection(QString collection)
{
    if (m_collection == collection)
        return;

    m_collection = collection;
    emit collectionChanged(collection);
    tryInit();
    emit fetch();
}

void BaseModel::insert(QJSValue value, int row)
{
    QEjdbItemModel::insert(m_client->convert(value), row);
}

