#include "qejdbitemmodel.h"
#include "qbson/qbsonobject.h"
#include "qbsonitemmodel_p.h"
#include "qejdbcollectionsync.h"

QEjdbItemModel::QEjdbItemModel(QEjdbDatabase db, QString collection, QObject *parent)
    : QAbstractListModel(parent), m_modeSync(0), m_bsonModel(0)
{
    initCollectionSyncModel(db, collection);
}

QEjdbItemModel::QEjdbItemModel(QObject *parent)
    : QAbstractListModel(parent), m_modeSync(0), m_bsonModel(0)
{
}

QEjdbItemModel::~QEjdbItemModel()
{
    delete m_modeSync;
}

/**
 * @brief Return value for index.
 */
QVariant QEjdbItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() && !isValid())
         return QVariant();

    return m_bsonModel->data(index.row(), role).toVariant();
}

/**
 * @brief Returns row count.
 */
int QEjdbItemModel::rowCount(const QModelIndex &parent) const
{
    if (isValid()) {
        return m_bsonModel->count();
    }
    return 0;
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
    if (isValid()) {
        m_bsonModel->setData(index.row(), role, QBsonValue::fromVariant(value));
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
 * @brief Inserts a bson object on given row.
 * @return void
 */
void QEjdbItemModel::insert(const QBsonObject &bsonObject, int row)
{
    if (isValid()) {
        beginInsertRows(QModelIndex(), row, row);
        m_bsonModel->insert(bsonObject, row);
        endInsertRows();
    }
}

/**
 * @brief Init model with collection and db to view all items from a collection.
 * @return void
 */
void QEjdbItemModel::initCollectionSyncModel(QEjdbDatabase db, const QString &collection)
{
    QEjdbCollectionSync *colSync = 0;
    if (m_modeSync) {
        delete m_modeSync;
    }

    colSync = new QEjdbCollectionSync(db);
    colSync->setCollection(collection);
    colSync->fetch();
    m_modeSync = colSync;
    m_bsonModel = colSync->model();
}

/**
 * @brief check if model is valid. Tests sync model.
 * @return void
 */
bool QEjdbItemModel::isValid() const
{
    return m_modeSync != 0 && m_bsonModel != 0;
}


QVariant QEjdbItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

void QEjdbItemModel::setCollection(const QString &collectionName)
{
    beginResetModel();
    endResetModel();
}

QHash<int, QByteArray> QEjdbItemModel::roleNames() const
{
    return m_bsonModel->roles();
}





