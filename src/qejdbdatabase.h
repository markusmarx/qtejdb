#ifndef QEJDBDATABASE_H
#define QEJDBDATABASE_H
#include <QString>
#include <QSharedPointer>
#include <QDir>

#include "ejdb.h"

#include "qejdbcollection.h"

namespace QEJDB {

    /**
     * enums
     */
    enum DB_MODE {
        /**< Open as a reader. */
        READ        = JBOREADER,
        /**< Open as a writer. */
        WRITE       = JBOWRITER,
         /**< Create if db file not exists. */
        CREATE      = JBOCREAT,
        /**< Truncate db on open. */
        TRUNCATE    = JBOTRUNC,
        /**< Open without locking. */
        NO_LOCK     = JBONOLCK,
        /**< Lock without blocking. */
        LOCK_NB     = JBOLCKNB,
        /**< Synchronize every transaction. */
        SYNC        = JBOTSYNC
    };

}


class QEJDBDatabase
{

public:

    /**
     * @brief QEJDBDatabase constructor
     */
    QEJDBDatabase();

    /**
     * @brief ~QEJDBDatabase destructor
     */
    ~QEJDBDatabase();


    /**
     * @brief openDatabase open database.
     * @param path
     * @param databaseName
     * @param mode
     * @return
     */
    bool open(QString path, QString databaseName, int mode);

    /**
     * @brief closeDatabase close database.
     * @return
     */
    bool close();

    /**
     * @brief isDatabaseOpened checks if the database is opened.
     * @return
     */
    bool isOpen();

    /**
     * @brief remove database and all files.
     * @return
     */
    bool remove();

    /**
     * @brief collection
     * @param collectionName
     * @return
     */
    QEJDBCollection collection(QString collectionName);

    /**
     * @brief createCollection
     * @param collectionName
     * @return
     */
    QEJDBCollection createCollection(QString collectionName);

private:

    /**
     * @brief m_dbPath contains the path to the db files.
     */
    QDir m_dbPath;

    /**
     * @brief m_dbName contains the db name.
     */
    QString m_dbName;

    /**
     * @brief m_db pointer to ejdb.
     */
    EJDB* m_db;

    /**
     * @brief m_collections
     */
    QHash<QString, QSharedPointer<QEJDBCollection> > m_collections;


    /**
     * @brief storeCollection store collection in hashmap and create QEJDBCollection object.
     * @param col
     * @param collectionName
     * @return
     */
    QEJDBCollection storeCollection(EJCOLL* col, QString collectionName);

    /**
     * @brief loadCollections when db is opened the collections are loaded.
     */
    void loadCollections();



};

#endif // QEJDBDATABASE_H
