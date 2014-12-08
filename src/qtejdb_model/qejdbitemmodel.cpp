#include "qejdbitemmodel.h"
#include "qbsonobject.h"

/**
 * @brief The QBsonModelData class Container to store bson objects.
 */
class QBsonModelData {

public:

    QBsonModelData(
              const QString &colName
            , const QBsonObject &item
            , const QBsonObject &parent = QBsonObject()
    ):collectionName(colName), bsonItem(item), bsonParent(parent)
    ,dirty(false), loaded(true){}

    QBsonModelData(const QString &colName, const QString &oid)
        :collectionName(colName), dirty(false), loaded(false)
    {}

    /**
     * @brief bsonParent it a placeholder. i think about.
     */
    QBsonObject bsonParent;

    /**
     * @brief bsonItem reference to bson item.
     */
    QBsonObject bsonItem;

    /**
     * @brief collectionName collection where the bsonItem is stored.
     */
    QString collectionName;

    /**
     * @brief loaded true if object loaded.
     */
    bool loaded;

    /**
     * @brief dirty true if object dirty.
     */
    bool dirty;

    /**
     * @brief oid object id of bson.
     */
    QString bsonOid;

};

/**
 * @brief The QEjdbItemModelWorker class encapsulate database functions.
 */
class QEjdbItemModelWorker
{


public:

    QEjdbItemModelWorker(QEjdbDatabase db):m_db(db)
    {
    }


    /**
     * @brief loadCollection loads all documents from collection.
     *
     * @param collectionName collection
     * @param query query object
     */
    void loadCollection(
            const QString &collectionName,
            const QBsonObject &query = QBsonObject())
    {
        QEjdbResult allDocuments = m_db.loadAll(collectionName);
        QList<QBsonObject>::iterator it;
        for(int i = 0; i < allDocuments.count(); i++) {
            m_loadedBsonList.append(
                        QSharedPointer<QBsonModelData>(
                            new QBsonModelData(
                                collectionName
                                , allDocuments.values().at(i))
                        )
            );
        }
    }

    QBsonModelData *modelData(int row)
    {
        if (row >= 0 && row < m_loadedBsonList.size()) {
            return m_loadedBsonList.at(row).data();
        }
        return 0;
    }

    /**
     * @brief rowCount Returns row count.
     *
     * @param parentModelData reference to parent or 0 if no parent.
     *
     * @return row count.
     */
    int rowCount(QBsonModelData *parentModelData=0)
    {
        Q_UNUSED(parentModelData)
        return m_loadedBsonList.size();
    }

private:

    /**
     * @brief m_db refence to @class QEjdbDatabase.
     */
    QEjdbDatabase m_db;

    /**
     * @brief m_loadedBsonList stores all loaded bsons.
     */
    QList<QSharedPointer<QBsonModelData> > m_loadedBsonList;


};

QEjdbItemModel::QEjdbItemModel(QEjdbDatabase db, QObject *parent) :
    QAbstractItemModel(parent), m_db(db),
    m_modelWorker(new QEjdbItemModelWorker(m_db))
{
}

QEjdbItemModel::~QEjdbItemModel()
{
    delete m_modelWorker;
}

/**
 * @brief QEjdbItemModel::data Return value for index.
 *
 * @param index
 * @param role
 * @return
 */
QVariant QEjdbItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
         return QVariant();

    QByteArray nameArr = m_roleNamesMap.value(role);

    QBsonValue value = getQBson(index).value(nameArr);

    return value.toVariant();
}

/**
 * @brief QEjdbItemModel::index Returns index
 *
 * @param row row number
 * @param column column number
 * @param parent is currently ignored.
 * @return
 */
QModelIndex QEjdbItemModel::index(int row, int column, const QModelIndex &parent) const
{

    //if (parent.isValid() && parent.column() != 0)
    //         return QModelIndex();

    //TreeItem *parentItem = getItem(parent);

    QBsonModelData *modelData = m_modelWorker->modelData(row);
    if (modelData)
         return createIndex(row, column, modelData);
    else
        return QModelIndex();

}

/**
 * @brief QEjdbItemModel::parent Returns parent of a item. Currently its not
 * implemented and returns a invalid index.
 *
 * @param index
 *
 * @return
 */
QModelIndex QEjdbItemModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int QEjdbItemModel::rowCount(const QModelIndex &parent) const
{
    return m_modelWorker->rowCount();
}

/**
 * @brief QEjdbItemModel::setData set data at given position.
 *
 * @param index index position
 * @param value new value
 * @param role role under which the value is stored
 *
 * @return true if succed or false in case of error.
 */
bool QEjdbItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    QBsonModelData *data = getQBsonModelData(index);

    if (data) {
        const QString valueName(QString(m_roleNamesMap.value(role)));
        data->bsonItem.insert(valueName, QBsonValue::fromVariant(value));
        emit dataChanged(createIndex(index.row(), 0), createIndex(index.row(), 0));
        return true;
    }
    return false;
}

bool QEjdbItemModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    return false;
}

bool QEjdbItemModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    return false;
}

int QEjdbItemModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

/**
 * @brief QEjdbItemModel::setNames set attribut names.
 *
 * @param names
 * @param namesMap
 */
void QEjdbItemModel::setNames(QStringList names, QHash<QString, QString> namesMap)
{
    int i = Qt::UserRole + 100;
    QHash<int, QByteArray> roles;

    foreach (QString name, names) {

        if (namesMap.contains(name)) {
            m_roleNamesMap.insert(i, namesMap.value(name).toLatin1());
        } else {
            m_roleNamesMap.insert(i, name.toLatin1());
        }

        roles.insert(i, name.toLatin1());
        i++;
    }
    m_roleNames = roles;
}

QVariant QEjdbItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

void QEjdbItemModel::setCollection(const QString &collectionName)
{
    beginResetModel();
    m_modelWorker->loadCollection(collectionName);

    endResetModel();
}

QHash<int, QByteArray> QEjdbItemModel::roleNames() const
{
    return m_roleNames;
}

QBsonObject QEjdbItemModel::getQBson(const QModelIndex &index) const
{
    if (QBsonModelData *data = getQBsonModelData(index)) {
        return data->bsonItem;
    }
    return QBsonObject();
}

QBsonModelData *QEjdbItemModel::getQBsonModelData(const QModelIndex &index) const
{
    if (index.isValid()) {
         QBsonModelData *bsonModelData = static_cast<QBsonModelData*>(index.internalPointer());
         if (bsonModelData) return bsonModelData;
    }
    return 0;
}



