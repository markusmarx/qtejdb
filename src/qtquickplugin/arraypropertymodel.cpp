#include "arraypropertymodel.h"
#include "itemmodel/qejdbarraypropertysync.h"
#include <QJSValueIterator>
#include <QDebug>

ArrayPropertyModel::ArrayPropertyModel(QObject *parent)
    :BaseModel(parent), m_parentObject(QJSValue(QJSValue::NullValue))
{

}

void ArrayPropertyModel::classBegin()
{

}

void ArrayPropertyModel::componentComplete()
{

}


bool ArrayPropertyModel::checkProperties()
{
    bool error = false;

    if (m_collection.isEmpty()) {
        error = true;
    }
    if (m_client == 0) {
        error = true;
    }
    if (m_arrayProperty.isEmpty()) {
        error = true;
    }
    if (m_parentObject.isNull()) {
        error = true;
    }

    return !error;
}

void ArrayPropertyModel::tryInit()
{
    if (checkProperties()) {
        m_client->registerModel(this);
    }
}

QString ArrayPropertyModel::arrayProperty() const
{
    return m_arrayProperty;
}

QString ArrayPropertyModel::propertyCollection() const
{
    return m_propertyCollection;
}

QJSValue ArrayPropertyModel::parentObject() const
{
    return m_parentObject;
}

void ArrayPropertyModel::connected()
{
    if (m_client->isConnected() && checkProperties()) {
        QEjdbDatabase db = QEjdbDatabase::database(m_client->connectionName());
        QBsonObject parentObject = m_client->convert(m_parentObject).toObject();

        QEjdbArrayPropertySync *arrPropSync
                = new QEjdbArrayPropertySync(
                        db, m_collection, m_arrayProperty,
                        m_propertyCollection, parentObject, this
                    );

        QEjdbItemModel::setSync(arrPropSync);

        QObject::connect(this, &ArrayPropertyModel::collectionChanged,
                         arrPropSync, &QEjdbArrayPropertySync::setCollection
                         );
        QObject::connect(this, &ArrayPropertyModel::fetch,
                         arrPropSync, &QEjdbArrayPropertySync::fetch
                         );
        QObject::connect(this, &ArrayPropertyModel::propertyCollectionChanged,
                         arrPropSync, &QEjdbArrayPropertySync::setPropertyCollection
                         );
        QObject::connect(this, &ArrayPropertyModel::arrayPropertyChanged,
                         arrPropSync, &QEjdbArrayPropertySync::setArrayProperty
                         );
        QObject::connect(this, &ArrayPropertyModel::parentObjectChanged,
                         arrPropSync, &QEjdbArrayPropertySync::setParentObject
                         );
        return;
    }
}

void ArrayPropertyModel::disconnected()
{

}

void ArrayPropertyModel::setArrayProperty(QString arrayProperty)
{
    if (m_arrayProperty == arrayProperty)
        return;

    m_arrayProperty = arrayProperty;
    emit arrayPropertyChanged(arrayProperty);
    tryInit();
}

void ArrayPropertyModel::setPropertyCollection(QString propertyCollection)
{
    if (m_propertyCollection == propertyCollection)
        return;

    m_propertyCollection = propertyCollection;
    emit propertyCollectionChanged(propertyCollection);
    tryInit();
}

void ArrayPropertyModel::setParentObject(QJSValue parentObject)
{
    if (m_parentObject.equals(parentObject))
        return;

    m_parentObject = parentObject;
    emit parentObjectChanged(m_client->convert(parentObject).toObject());
    tryInit();
}

