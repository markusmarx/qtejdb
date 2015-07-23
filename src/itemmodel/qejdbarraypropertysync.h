#ifndef QEJDBARRAYPROPERTYSYNC_H
#define QEJDBARRAYPROPERTYSYNC_H
#include "qbsonitemmodelsync_p.h"
#include "qtejdb/qejdbdatabase.h"
#include "qbson/qbsonobject.h"

class QEjdbArrayPropertySync: public QEjdbAbstractSync
{
    Q_OBJECT
public:
    explicit QEjdbArrayPropertySync(QEjdbDatabase db,
                                    QString collection = QString(),
                                    QString property = QString(),
                                    QString propertyCollection = QString(),
                                    QBsonObject bsonObject = QBsonObject(),
                                    QObject *parent = 0);
    ~QEjdbArrayPropertySync();
    QBsonObject bsonObject();
    QString propertyName();
    QString collection();
    QString propertyCollection();

public slots:

    void fetch();
    void setPropertyCollection(QString propertyCollection);
    void setCollection(QString collection);
    void setBsonObject(QBsonObject bsonObject, QString propertyName);
    void setParentObject(QBsonObject bsonObject);
    void setArrayProperty(QString arrayProperty);
    void itemRemoved(int row, QBsonObject removedObject);
    void itemInserted(int row);
    void itemMoved(int sourceRow, int destinationRow);
    void itemUpdated(QString property, QVariant value, int row);

private:

    QEjdbDatabase m_db;
    QBsonObject m_parentObject;
    QString m_arrayProperty;
    QString m_collection;
    QString m_propertyCollection;

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
        if (m_parentObject.contains(m_arrayProperty)
                && m_parentObject.value(m_arrayProperty).isArray()) {
            return m_parentObject.value(m_arrayProperty).toArray();
        }
        QBsonArray array;
        m_parentObject.insert(m_arrayProperty, array);
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
#endif // QEJDBARRAYPROPERTYSYNC_H
