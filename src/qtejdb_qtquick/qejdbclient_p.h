#ifndef QEJDBCLIENTPRIVATE_H
#define QEJDBCLIENTPRIVATE_H

#include <QtGlobal>
#include "qejdbdatabase.h"

class QEjdbClient;
class QEjdbClientPrivate {
public:
    QEjdbClientPrivate(QEjdbClient* q): q_ptr(q)
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
     * @brief m_bsonConverter wraps the logic to convert QJSValue to QBsonValue
     * and visa versa.
     */
    QBsonConverter m_bsonConverter;

    /**
     * @brief checkCollection creates the collection of not exist.
     *
     * @param db reference to database
     * @param collection collection Name
     */
    inline void checkCollection(QEjdbDatabase &db, QString collection)
    {
        if (db.isOpen() && !db.containsCollection(collection)) {
            db.createCollection(collection);
        }
    }
    /**
     * @brief database get the database connectionName
     *
     * @return QEjdbDatabase
     */
     inline QEjdbDatabase database() {

         return QEjdbDatabase::database(m_connectionName);
     }


     QBsonObject convert(const QJSValue &jsValue);
     QJSValue convert(const QBsonObject &bsonObject);
     QJSValue save(QString collectionName, const QJSValue &jsValue);
     QJSValue load(QString collectionName, QJSValue uid);
};

#endif // QEJDBCLIENTPRIVATE_H

