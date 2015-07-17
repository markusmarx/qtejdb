#ifndef QBSONMODELSYNC_P_H
#define QBSONMODELSYNC_P_H
#include "qbsonitemmodel_p.h"
#include <QByteArray>
#include "qtejdb/qejdbdatabase.h"
#include "qbson/qbsonobject.h"
#include "qbson/qbsonarray.h"


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
#endif // QBSONMODELSYNC_P_H

