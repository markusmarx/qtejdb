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
void QBsonItemModel::insert(QBsonObject &bsonObject, const uint &row)
{
    if (row <= count()) {
        internalInsert(bsonObject, row);
        emit itemInserted(row);
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
    internalClear();
    QList<QBsonObject>::iterator it;
    int i = 0;
    for (it = bsonList.begin(); it != bsonList.end(); it++) {
        internalInsert(*it, i++);
    }
    buildRoles();
    emit reset();
}

/**
 * @brief QBsonItemModel::set set a bson listt
 */
void QBsonItemModel::set(QList<QBsonValue> bsonList)
{
    internalClear();
    QList<QBsonValue>::iterator it;
    int i = 0;
    for (it = bsonList.begin(); it != bsonList.end(); it++) {
        QBsonObject obj = (*it).toObject();
        internalInsert(obj, i++);
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
    : m_db(db), m_qBsonItemModel(new QBsonItemModel(this)), QObject(parent)
{

    QObject::connect(m_qBsonItemModel, &QBsonItemModel::itemInserted,
                     this, &QEjdbCollectionSync::itemSave);
    QObject::connect(m_qBsonItemModel, &QBsonItemModel::itemRemoved,
                     this, &QEjdbCollectionSync::itemRemoved);
    QObject::connect(m_qBsonItemModel, &QBsonItemModel::itemUpdated,
                     this, &QEjdbCollectionSync::itemUpdated);

}

/**
 * @brief QEjdbCollectionSync::~QEjdbCollectionSync deletes bsonModel
 */
QEjdbCollectionSync::~QEjdbCollectionSync()
{
    delete m_qBsonItemModel;
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
 * @brief QEjdbCollectionSync::model returns the bson model that manages the
 * bson objects.
 */
QBsonItemModel *QEjdbCollectionSync::model()
{
    return m_qBsonItemModel;
}

/**
 * @brief QEjdbCollectionSync::fetch get data from collection with query and
 * hints.
 */
void QEjdbCollectionSync::fetch()
{
    if (isDbValid()) {
        QEjdbResult result = m_db.query(m_collection, m_query);
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
void QEjdbCollectionSync::itemSave(int row)
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

/**
 * @brief QEjdbArrayPropertySync::bsonObject returns bson object that contains
 * the array property stored in propertyName.
 */
QEjdbArrayPropertySync::QEjdbArrayPropertySync(QEjdbDatabase db, QObject *parent)
    : m_qBsonItemModel(new QBsonItemModel(this)), m_db(db), QObject(parent)
{
    QObject::connect(m_qBsonItemModel, &QBsonItemModel::itemInserted,
                     this, &QEjdbArrayPropertySync::itemInserted);
    QObject::connect(m_qBsonItemModel, &QBsonItemModel::itemRemoved,
                     this, &QEjdbArrayPropertySync::itemRemoved);
    QObject::connect(m_qBsonItemModel, &QBsonItemModel::itemUpdated,
                     this, &QEjdbArrayPropertySync::itemUpdated);
    QObject::connect(m_qBsonItemModel, &QBsonItemModel::itemMoved,
                     this, &QEjdbArrayPropertySync::itemMoved);
}

/**
 * @brief QEjdbArrayPropertySync::~QEjdbArrayPropertySync delete bsonmodel.
 */
QEjdbArrayPropertySync::~QEjdbArrayPropertySync()
{
    delete m_qBsonItemModel;
}

/**
 * @brief QEjdbCollectionSync::model returns the bson model that manages the
 * bson objects.
 */
QBsonItemModel *QEjdbArrayPropertySync::model()
{
   return m_qBsonItemModel;
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
    return m_propertyName;
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
        QBsonObject query;
        QBsonArray andQ;
        andQ.append(QBsonObject("_id", m_parentObject.oid()));
        query.append("$do", QBsonObject(
                              m_propertyName, QBsonObject(
                                  "$join", m_propertyCollection)
                              )
                    );
        query.append("$and", andQ);
        QEjdbResult result = m_db.query(m_collection, query);

        if (result.count() > 0) {
            QBsonObject parentObject = result.first();

            QBsonArray array = parentObject.value(m_propertyName).toArray();
            m_qBsonItemModel->set(array.values());
            return;
        }

        QBsonArray array = getBsonArray();
        m_qBsonItemModel->set(array.values());
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
    m_parentObject = bsonObject;
    m_propertyName = propertyName;
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

    }
}

/**
 * @brief QEjdbArrayPropertySync::itemInserted slot to insert a new bson object.
 */
void QEjdbArrayPropertySync::itemInserted(int row)
{
    if (isDbValid()) {
        QBsonObject newObject = m_qBsonItemModel->row(row);
        if (newObject.hasOid()) {
            QBsonObject obj = m_db.load(m_collection, m_parentObject.oid());
            if (obj.hasOid()) {
                m_parentObject = obj;
            }
            m_db.save(m_propertyCollection, newObject);
            QBsonArray array = getBsonArray();
            if (row >= 0 && row <= array.size()) {
                array.insert(row, QBsonOid(newObject.oid()));
                m_db.save(m_collection, m_parentObject);
            }
        }
    }
}

/**
 * @brief QEjdbArrayPropertySync::itemMoved slot to move a bson object on array.
 */
void QEjdbArrayPropertySync::itemMoved(int sourceRow, int destinationRow)
{
    if (isDbValid()) {
        m_parentObject = m_db.load(m_collection, m_parentObject.oid());
        QBsonArray array = getBsonArray();
        array.insert(destinationRow, array.take(sourceRow).toId());
        m_db.save(m_collection, m_parentObject);
    }
}

/**
 * @brief QEjdbArrayPropertySync::itemUpdated slot to update a document. Saves
 * only object have a oid.
 */
void QEjdbArrayPropertySync::itemUpdated(QString property, QVariant value, int row)
{
    if (isDbValid()) {
        QBsonObject newObject = m_db.load(m_propertyCollection, m_qBsonItemModel->row(row).oid());
        if (newObject.hasOid()) {
            newObject.insert(property, QBsonValue::fromVariant(value));
            m_db.save(m_propertyCollection, newObject);
        }
    }
}
