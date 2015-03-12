#ifndef QBSONITEMMODEL_P_H
#define QBSONITEMMODEL_P_H

#include <QObject>
#include <QList>
#include <QHash>
#include <QByteArray>
#include "qejdbdatabase.h"
#include "qbsonobject.h"
#include "qbsonarray.h"


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
    void set(QList<QBsonValue> bsonList);
    void insert(QBsonObject &bsonObject, const uint &row);
    void update(QString property, QVariant value, const uint &row);
    void append(QBsonObject bsonObject);
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
    void itemUpdated(QString property, QVariant value, uint row);
    void itemRemoved(uint row, QBsonObject bsonObject);
    void reset();

private:

    QList<StorageId> m_bsonList;
    QHash<QBsonId, StorageId> m_bsonId;
    QHash<StorageId, QBsonObject> m_bsonObjects;
    QHash<int, QByteArray> m_roles;

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
    inline void internalInsert(QBsonObject &bsonObject, uint row)
    {
        QBsonOid id = bsonObject.oid();
        StorageId storageId = id.hash();
        m_bsonList.insert(row, storageId);
        m_bsonId.insert(id.toString(), storageId);
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

class QEjdbCollectionSync: public QObject
{
    Q_OBJECT
public:
    explicit QEjdbCollectionSync(QEjdbDatabase db, QObject *parent = 0);
    ~QEjdbCollectionSync();
    QBsonObject query() const;
    QBsonObject hints() const;
    QString collection() const;
    QBsonItemModel* model();

public slots:
    void fetch();
    void setQuery(QBsonObject query);
    void setHints(QBsonObject hints);
    void setCollection(QString collection);

private slots:
    void itemRemoved(int row, QBsonObject removedObject);
    void itemSave(int row);
    void itemUpdated(QString property, QVariant value, int row);

private:
    QEjdbDatabase m_db;
    QBsonItemModel *m_qBsonItemModel;
    QString m_collection;
    QBsonObject m_query;
    QBsonObject m_hints;

    bool isDbValid()
    {
        return m_db.isOpen()
                && m_db.containsCollection(m_collection);
    }
};

class QEjdbArrayPropertySync: public QObject
{
    Q_OBJECT
public:
    explicit QEjdbArrayPropertySync(QEjdbDatabase db, QObject *parent = 0);
    ~QEjdbArrayPropertySync();
    QBsonItemModel *model();
    QBsonObject bsonObject();
    QString propertyName();
    QString collection();
    QString propertyCollection();

public slots:

    void fetch();
    void setPropertyCollection(QString propertyCollection);
    void setCollection(QString collection);
    void setBsonObject(QBsonObject bsonObject, QString propertyName);

private slots:
    void itemRemoved(int row, QBsonObject removedObject);
    void itemInserted(int row);
    void itemMoved(int sourceRow, int destinationRow);
    void itemUpdated(QString property, QVariant value, int row);

private:

    QEjdbDatabase m_db;
    QBsonItemModel *m_qBsonItemModel;
    QBsonObject m_parentObject;
    QString m_propertyName;
    QString m_collection;
    QString m_propertyCollection;

    QBsonObject loadBson(const QString &collection, const QBsonOid &oid);
    QBsonObject reloadBson(const QString &collection, const QBsonObject &bson);
    void saveBson(const QString &collection, QBsonObject &object);

    /**
     * @brief isDbValid check all required values.
     */
    inline bool isDbValid()
    {
        return m_db.isOpen()
                && m_db.containsCollection(m_collection)
                && ( m_propertyCollection.isEmpty()
                        || m_db.containsCollection(m_propertyCollection));
    }

    /**
     * @brief getBsonArray returns the bson array.
     */
    inline QBsonArray getBsonArray()
    {
        m_parentObject = reloadBson(m_collection, m_parentObject);
        if (m_parentObject.contains(m_propertyName)
                && m_parentObject.value(m_propertyName).isArray()) {
            return m_parentObject.value(m_propertyName).toArray();
        }
        QBsonArray array;
        m_parentObject.insert(m_propertyName, array);
        return array;
    }

    /**
     * @brief isJoined returns true if propertyCollection is not empty.
     */
    inline bool isJoined()
    {
        return !m_propertyCollection.isEmpty();
    }

};


#endif // QBSONITEMMODEL_P_H
