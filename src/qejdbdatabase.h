#ifndef QEJDBDATABASE_H
#define QEJDBDATABASE_H
#include <QString>
#include <QSharedPointer>
#include <QDir>

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

class QEjdbDatabasePrivate;
class QEjdbDatabasePrivate;

class QEjdbDatabase
{

public:

    QEjdbDatabase();

    /**
     * @brief ~QEJDBDatabase destructor
     */
    ~QEjdbDatabase();


    /**
     * @brief openDatabase open database.
     * @param path
     * @param databaseName
     * @param mode
     * @return
     */
    bool open();

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
     * @brief collection
     * @param collectionName
     * @return
     */
    QEjdbCollection collection(QString collectionName);

    /**
     * @brief createCollection
     * @param collectionName
     * @return
     */
    QEjdbCollection createCollection(QString collectionName);

    static QEjdbDatabase addDatabase(QString path, QString database, int mode, QString connectionName = QLatin1String(defaultConnection));

    static QEjdbDatabase database(QString connectionName = QLatin1String(defaultConnection));

    static void removeDatabase(QString connectionName  = QLatin1String(defaultConnection));

    static bool removeDatabaseFiles(QString path, QString database);

    QEjdbDatabase(const QEjdbDatabase &other);

    QEjdbDatabase &operator =(const QEjdbDatabase &other);

protected:
    /**
     * @brief QEJDBDatabase constructor
     * @param connectionName
     */
    QEjdbDatabase(QString path, QString databaseName, int mode);


private:
    friend class QEjdbDatabasePrivate;
    QEjdbDatabasePrivate *d;

    QT_STATIC_CONST char *defaultConnection;

    /**
     * @brief storeCollection store collection in hashmap and create QEJDBCollection object.
     * @param col
     * @param collectionName
     * @return
     */
    QEjdbCollection storeCollection(EJCOLL* col, QString collectionName);

    /**
     * @brief loadCollections when db is opened the collections are loaded.
     */
    void loadCollections();


};

#endif // QEJDBDATABASE_H
