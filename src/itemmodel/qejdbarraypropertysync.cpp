#include "qejdbarraypropertysync.h"
/**
 * @class QEjdbArrayPropertySync
 * @brief QEjdbArrayPropertySync manages a array property.
 *
 * interface requirements
 * ======================
 * - set bson object and re load it from db
 * - set id and load it from db
 * - if propertyCollection is set then make a join over
 * property and propertyCollection
 * - save a property bson on a own collection only propertyCollection is set
 * - if propertyCollection not set the bson is saved as subobj of parent object
 *
 */

/**
 * @brief QEjdbArrayPropertySync::bsonObject returns bson object that contains
 * the array property stored in propertyName.
 */
QEjdbArrayPropertySync::QEjdbArrayPropertySync(QEjdbDatabase db,
                                               QString collection,
                                               QString property,
                                               QString propertyCollection,
                                               QBsonObject bsonObject,
                                               QObject *parent)
    : m_db(db), m_collection(collection), m_arrayProperty(property)
    , m_propertyCollection(propertyCollection), m_parentObject(bsonObject),
      QEjdbAbstractSync(parent)
{

}

/**
 * @brief QEjdbArrayPropertySync::~QEjdbArrayPropertySync delete bsonmodel.
 */
QEjdbArrayPropertySync::~QEjdbArrayPropertySync()
{
}


QBsonObject QEjdbArrayPropertySync::bsonObject()
{
    return m_parentObject;
}

/**
 * @brief QEjdbArrayPropertySync::propertyName returns the property name under
 * which the array is stored.
 */
QString QEjdbArrayPropertySync::propertyName()
{
    return m_arrayProperty;
}

/**
 * @brief QEjdbArrayPropertySync::collection is the collection in which the
 * bson object is stored.
 */
QString QEjdbArrayPropertySync::collection()
{
    return m_collection;
}

/**
 * @brief QEjdbArrayPropertySync::propertyCollection returns property collection.
 * The collection in which the bsonobjects in property are stored.
 */
QString QEjdbArrayPropertySync::propertyCollection()
{
    return m_propertyCollection;
}

/**
 * @brief QEjdbArrayPropertySync::fetch fetch the object and add it to model.
 */
void QEjdbArrayPropertySync::fetch()
{
    if (isDbValid()) {

        QBsonObject found;
        if (isJoined()) {
            QBsonObject query;
            QBsonArray andQ;
            andQ.append(QBsonObject("_id", m_parentObject.oid()));
            query.append("$do", QBsonObject(
                                  m_arrayProperty, QBsonObject(
                                      "$join", m_propertyCollection)
                                  )
                        );
            query.append("$and", andQ);
            QEjdbResult result = m_db.query(m_collection, query);

            if (result.count() > 0) {
                found = result.first();
            }

        } else {
            found = m_db.load(m_collection, m_parentObject.oid());
            m_parentObject = found;
        }

        if (found.hasOid()) {
            QBsonArray array = found.value(m_arrayProperty).toArray();
            m_qBsonItemModel->clear();
            QList<QBsonObject> objectList;
            foreach (QBsonValue val, array.values()) {
                if (val.isObject()) {
                    objectList << val.toObject();
                }
            }
            m_qBsonItemModel->set(objectList);
        }

    }
}

/**
 * @brief QEjdbArrayPropertySync::setPropertyCollection set a collection for
 * property. @see QEjdbArrayPropertySync::propertyCollection()
 */
void QEjdbArrayPropertySync::setPropertyCollection(QString propertyCollection)
{
    m_propertyCollection = propertyCollection;
}

/**
 * @brief QEjdbArrayPropertySync::setCollection set collection in which the bson
 * object is stored. @see QEjdbArrayPropertySync::collection()
 */
void QEjdbArrayPropertySync::setCollection(QString collection)
{
    m_collection = collection;
}

/**
 * @brief QEjdbArrayPropertySync::setBsonObject the bson object that contains
 * the array property. @see QEjdbArrayPropertySync::bsonObject()
 */
void QEjdbArrayPropertySync::setBsonObject(QBsonObject bsonObject, QString propertyName)
{
    setParentObject(bsonObject);
    setArrayProperty(propertyName);
}

void QEjdbArrayPropertySync::setParentObject(QBsonObject parentObject)
{
    m_parentObject = parentObject;
}

void QEjdbArrayPropertySync::setArrayProperty(QString arrayProperty)
{
    m_arrayProperty = arrayProperty;
}

/**
 * @brief QEjdbArrayPropertySync::itemRemoved slot to remove a object from
 * array property.
 */
void QEjdbArrayPropertySync::itemRemoved(int row, QBsonObject removedObject)
{
    Q_UNUSED(removedObject)
    if (isDbValid()) {
        QBsonArray array = getBsonArray();
        array.remove(row);
        m_db.save(m_collection, m_parentObject);
        m_db.remove(m_propertyCollection, removedObject);
    }
}

/**
 * @brief QEjdbArrayPropertySync::itemInserted slot to insert a new bson object.
 */
void QEjdbArrayPropertySync::itemInserted(int row)
{
    if (isDbValid()) {
        QBsonObject newObject = m_qBsonItemModel->row(row);
        QBsonArray array = getBsonArray();
        if (row >= 0 && row <= array.size()) {
            if (isJoined()) {
                m_db.save(m_propertyCollection, newObject);
                array.insert(row, newObject.oid());
            } else {
                array.insert(row, newObject);
            }
            m_parentObject.insert(m_arrayProperty, array);
            m_db.save(m_collection, m_parentObject);
        }
    }
}

/**
 * @brief QEjdbArrayPropertySync::itemMoved slot to move a bson object on array.
 */
void QEjdbArrayPropertySync::itemMoved(int sourceRow, int destinationRow)
{
    m_parentObject = m_db.load(m_collection, m_parentObject.oid());
    QBsonArray array = getBsonArray();
    array.insert(destinationRow, array.take(sourceRow));
    saveBson(m_collection, m_parentObject);
}

/**
 * @brief QEjdbArrayPropertySync::itemUpdated slot to update a document. Saves
 * only object have a oid.
 */
void QEjdbArrayPropertySync::itemUpdated(QString property, QVariant value, int row)
{
    QBsonObject updateObject = reloadBson(m_propertyCollection, m_qBsonItemModel->row(row));
    updateObject.insert(property, QBsonValue::fromVariant(value));
    if (isJoined()) {
        saveBson(m_propertyCollection, updateObject);
    } else {
        QBsonArray array = getBsonArray();
        array.replace(row, updateObject);
        saveBson(m_collection, m_parentObject);
    }

}

/**
 * @brief QEjdbArrayPropertySync::reloadBson loads a bson with _id from given bson
 * from database. If database is not valid or bson have no oid property returns
 * the given bson.
 */
QBsonObject QEjdbArrayPropertySync::reloadBson(
        const QString &collection, const QBsonObject &bson
) {
    if (bson.hasOid() && isDbValid()) {
        QBsonObject obj = m_db.load(collection, bson.oid());
        if (obj.hasOid()) return obj;
    }
    return bson;
}

/**
 * @brief QEjdbArrayPropertySync::saveBson save the given bson.
 */
void QEjdbArrayPropertySync::saveBson(const QString &collection, QBsonObject &bson)
{
    if (isDbValid()) {
        m_db.save(collection, bson);
    }
}


