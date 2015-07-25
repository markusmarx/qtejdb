#ifndef QEJDBCLIENTPRIVATE_H
#define QEJDBCLIENTPRIVATE_H

#include <QtGlobal>
#include "qtejdb/qejdbdatabase.h"

class QEjdbClient;
class QEjdbClientPrivate {
public:
    QEjdbClientPrivate(QEjdbClient* q): q_ptr(q), m_isConnected(false)
    {
    }

    Q_DECLARE_PUBLIC(QEjdbClient)
    QEjdbClient* q_ptr;


    /**
     * @internal
     * @brief m_connectionName is the key to get database connection
     * from dictionary @see QEjdbDatabase::addDatabase()
     */
    QString m_connectionName;

    /**
     * @internal
     * @brief m_uri is the connection string @see QEjdbDatabase::addDatabase()
     */
    QString m_uri;

    /**
     * @internal
     * @brief m_autoCreateCollection if true create a collection if not exist
     */
    bool m_autoCreateCollection;

    /**
     * @internal
     * @brief m_autoCloseConnection if true the connection is closed and database
     * is removed.
     */
    bool m_autoCloseConnection;

    /**
     * @internal
     * @brief m_bsonConverter wraps the logic to convert QJSValue to QBsonValue
     * and visa versa.
     */
    QBsonConverter m_bsonConverter;

    /**
     * @brief m_isConnected indicated wether a client is connected or not.
     */
    bool m_isConnected;

    /**
     * @brief checkCollection creates the collection of not exist.
     *
     * @param db reference to database
     * @param collection collection Name
     */
    inline bool checkCollection(QEjdbDatabase &db, QString collection)
    {
        if (!db.isOpen()) {
            return false;
        }
        if (db.containsCollection(collection)) {
            return true;
        }
        return m_autoCreateCollection ? db.createCollection(collection) : false;
    }

    /**
     * @brief database get the database connectionName
     *
     * @return QEjdbDatabase
     */
     inline QEjdbDatabase database() {

         return QEjdbDatabase::database(m_connectionName);
     }


     QBsonValue convert(const QJSValue &jsValue);
     QJSValue convert(const QBsonObject &bsonObject);
     QJSValue save(QString collectionName, const QJSValue &jsValue);
     QJSValue load(QString collectionName, QJSValue uid);
     QJSValue remove(QString collectionName, QJSValue uid);
     QJSValue query(QString collectionName, QJSValue query, QJSValue hints);
};

#endif // QEJDBCLIENTPRIVATE_H

