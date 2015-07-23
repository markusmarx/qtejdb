#include "qejdbitemmodel.h"
#include "qbson/qbsonobject.h"
#include "qbsonitemmodel_p.h"
#include "qejdbcollectionsync.h"

QEjdbItemModel::QEjdbItemModel(QEjdbAbstractSync *sync, QObject *parent)
    : m_sync(sync), m_bsonModel(sync->model())
{
    resetModel();
}

QEjdbItemModel::QEjdbItemModel(QObject *parent)
    : QAbstractListModel(parent), m_sync(0), m_bsonModel(0)
{
}

QEjdbItemModel::~QEjdbItemModel()
{
    QObject::disconnect(m_bsonModel, &QBsonItemModel::reset, this, &QEjdbItemModel::reset);
    delete m_sync;
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
void QEjdbItemModel::insert(int row, const QBsonObject &bsonObject)
{
    if (isValid()) {
        beginInsertRows(QModelIndex(), row, row);
        m_bsonModel->insert(bsonObject, row);
        endInsertRows();
    }
}

void QEjdbItemModel::remove(int row, int count)
{
    if (isValid()) {
        beginRemoveRows(QModelIndex(), row, row);
        m_bsonModel->remove(row);
        endRemoveRows();
    }
}

void QEjdbItemModel::move(int sourceRow, int destinationRow, int count)
{
    if (isValid()
            && beginMoveRows(QModelIndex(), sourceRow, sourceRow, QModelIndex(), destinationRow)) {
        m_bsonModel->move(sourceRow, destinationRow);
        endMoveRows();
    }
}

QBsonObject QEjdbItemModel::get(int row)
{
    return m_bsonModel->row(row);
}

void QEjdbItemModel::reset()
{
    if (isValid()) {
        resetModel();
    }
}

/**
 * @brief check if model is valid. Tests sync model.
 * @return void
 */
bool QEjdbItemModel::isValid() const
{
    return m_sync != 0 && m_bsonModel != 0;
}

void QEjdbItemModel::resetModel()
{
    Q_ASSERT(m_sync);
    beginResetModel();
    m_sync->fetch();
    endResetModel();
}


QVariant QEjdbItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

void QEjdbItemModel::setSync(QEjdbAbstractSync *sync)
{
    if (qobject_cast<QObject*>(m_sync)) {
        QObject::disconnect(m_sync);
        m_sync->deleteLater();

    }
    m_sync = sync;
    m_bsonModel = sync->model();
    resetModel();
}

QHash<int, QByteArray> QEjdbItemModel::roleNames() const
{
    return m_bsonModel->roles();
}
