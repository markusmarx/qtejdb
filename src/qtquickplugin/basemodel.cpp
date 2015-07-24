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

int BaseModel::count() const
{
    return QEjdbItemModel::rowCount();
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

void BaseModel::insert(int row, QJSValue value)
{
    QEjdbItemModel::insert(row, m_client->convert(value).toObject());
}

void BaseModel::set(int row, QJSValue value)
{
    QEjdbItemModel::remove(row);
    QEjdbItemModel::insert(row, m_client->convert(value).toObject());
}

QJSValue BaseModel::get(int row)
{
    return m_client->convert(QEjdbItemModel::get(row));
}

void BaseModel::append(QJSValue value)
{
    QEjdbItemModel::insert(count(), m_client->convert(value).toObject());
}

void BaseModel::setProperty(int row, QString property, QJSValue value)
{
    QEjdbItemModel::setData(index(row),
                            m_client->convert(value).toVariant(),
                            roleNames().key(property.toLatin1())
                            );
}

