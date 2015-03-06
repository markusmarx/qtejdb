#ifndef QBSONITEMMODEL_P_H
#define QBSONITEMMODEL_P_H

#include <QObject>
#include <QList>
#include <QHash>
#include <QByteArray>
#include "qbsonobject.h"
/**
 * @internal
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

    void insert(QBsonObject &bsonObject, const uint &row);
    void update(QBsonObject &bsonObject, const uint &row);
    void append(QBsonObject &bsonObject);
    void move(int sourceRow, int destinationRow);
    void remove(int row);
    void buildRoles();
    QHash<int, QByteArray> roles();

    QBsonObject row(int row);
    QBsonObject oid(QBsonId bsonId);

    int count();


signals:

    void itemMoved(uint sourceRow, uint destinationRow);
    void itemInserted(uint row);
    void itemUpdated(uint row);
    void itemRemoved(QBsonObject bsonObject);
    void reset();

public slots:


private:

    QList<StorageId> m_bsonList;
    QHash<QBsonId, StorageId> m_bsonId;
    QHash<StorageId, QBsonObject> m_bsonObjects;
    QHash<int, QByteArray> m_roles;

    /**
     * @internal
     * @brief insertIntoList insert into internal structure.
     *
     * @param bsonObject bsonobject
     * @param row row
     */
    inline void internalInsert(QBsonObject &bsonObject, uint row)
    {
        if (bsonObject.contains("_id")) {
            QBsonOid id = bsonObject.value("_id").toId();
            StorageId storageId = id.hash();
            m_bsonList.insert(row, storageId);
            m_bsonId.insert(id.toString(), storageId);
            m_bsonObjects.insert(storageId, bsonObject);
        }
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
     * @brief internalGet Returns the bson on row.
     *
     * @param row row
     *
     * @return bson stored on row
     */
    inline QBsonObject internalGet(int row)
    {
        StorageId storageId = m_bsonList.value(row);
        return m_bsonObjects.value(storageId);
    }

    /**
     * @brief internalGet Returns bson by bsonId.
     *
     * @param bsonId bsonid
     *
     * @return bson stored on id
     */
    inline QBsonObject internalGet(QBsonId bsonId)
    {
        return m_bsonObjects.value(m_bsonId.value(bsonId));
    }

    inline bool isValidRow(int row)
    {
        return row >= 0 && row < count();
    }

};

#endif // QBSONITEMMODEL_P_H
