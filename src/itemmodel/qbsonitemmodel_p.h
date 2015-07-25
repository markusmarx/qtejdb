#ifndef QBSONITEMMODEL_P_H
#define QBSONITEMMODEL_P_H

#include <QObject>
#include <QList>
#include <QHash>
#include "qbson/qbsonobject.h"

/**
 * @internal
 * Stores bsonobjects in a model.
 */
class QBsonItemModel : public QObject
{
    Q_OBJECT
public:
    typedef QString QBsonId;
    typedef uint StorageId;

    explicit QBsonItemModel(QObject *parent = 0);
    ~QBsonItemModel();
    void set(QList<QBsonObject> bsonList);
    void insert(const QBsonObject &bsonObject, const uint &row);
    void update(QString property, QVariant value, const uint &row);
    void append(QBsonObject bsonObject);
    void move(int sourceRow, int destinationRow);
    void remove(int row);
    void buildRoles();
    void clear();
    QHash<int, QByteArray> roles();
    QBsonObject row(int row);
    QBsonValue data(int row, int role);
    void setData(int row, int role, const QBsonValue &value);
    QBsonObject byOid(QBsonId bsonId);
    int count();

signals:

    void itemMoved(uint sourceRow, uint destinationRow);
    void itemInserted(uint row);
    void itemUpdated(QString property, QVariant value, uint row);
    void itemRemoved(uint row, QBsonObject bsonObject);
    void reset();

private:

    QList<StorageId> m_bsonList;
    QHash<QBsonId, StorageId> m_bsonId;
    QHash<StorageId, QBsonObject> m_bsonObjects;
    QHash<int, QByteArray> m_roles;
    int m_idCounter;

    QString roleAsKey(int role);

    /**
     * @brief internalClear clear all lists.
     */
    void internalClear()
    {
        m_bsonId.clear();
        m_bsonList.clear();
        m_bsonObjects.clear();
    }

    /**
     * @internal
     * @brief insertIntoList insert into internal structure.
     *
     * @param bsonObject bsonobject
     * @param row row
     */
    inline void internalInsert(const QBsonObject &bsonObject, uint row)
    {
        StorageId storageId = bsonObject.hasOid()?bsonObject.oid().hash():m_idCounter++;
        m_bsonList.insert(row, storageId);
        m_bsonId.insert(bsonObject.oid().toString(), storageId);
        m_bsonObjects.insert(storageId, bsonObject);
    }

    /**
     * @brief removeFromList remove from internal structure.
     * @param row removed row.
     */
    inline QBsonObject internalRemove(int row)
    {
        StorageId storageId = m_bsonList.takeAt(row);
        QBsonId id = m_bsonId.key(storageId);
        m_bsonId.remove(id);
        QBsonObject bsonObject = m_bsonObjects.take(storageId);
        return bsonObject;
    }

    /**
     * @brief internalGetWWithRow Returns the bson on row.
     *
     * @param row row
     *
     * @return bson stored on row
     */
    inline QBsonObject internalGetWithRow(int row)
    {
        StorageId storageId = m_bsonList.value(row);
        return m_bsonObjects.value(storageId);
    }

    /**
     * @brief internalGetWithOid Returns bson by bsonId.
     *
     * @param bsonId bsonid
     *
     * @return bson stored on id
     */
    inline QBsonObject internalGetWithOid(QBsonId bsonId)
    {
        return m_bsonObjects.value(m_bsonId.value(bsonId));
    }

    /**
     * @brief isValidRow test if given row is in range.
     *
     * @param row row index beginning 0
     *
     * @return
     */
    inline bool isValidRow(int row)
    {
        return row >= 0 && row < count();
    }

};


#endif // QBSONITEMMODEL_P_H
