#ifndef QEJDBDATABASE_H
#define QEJDBDATABASE_H
#include <QString>
#include <QSharedPointer>
#include <QDir>
#include <QUrl>

#include "qejdbcollection.h"

namespace QEJDB {

    /**
     * enums
     */
    enum DB_MODE {
        /**< Open as a reader. */
        READ        = 1 << 0,
        /**< Open as a writer. */
        WRITE       = 1 << 1,
         /**< Create if db file not exists. */
        CREATE      = 1 << 2,
        /**< Truncate db on open. */
        TRUNCATE    = 1 << 3,
        /**< Open without locking. */
        NO_LOCK     = 1 << 4,
        /**< Lock without blocking. */
        LOCK_NB     = 1 << 5,
        /**< Synchronize every transaction. */
        SYNC        = 1 << 6
    };

}

class QEjdbDatabasePrivate;
class QEjdbQuery;
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
     * @brief createCollection
     * @param collectionName
     * @return
     */
    bool createCollection(const QString &collectionName);

    bool containsCollection(const QString &collectionName);

    static QEjdbDatabase addDatabase(QString url, int mode, QString connectionName = QLatin1String(defaultConnection));

    static QEjdbDatabase database(const QString &connectionName = QLatin1String(defaultConnection));

    static void removeDatabase(const QString &connectionName  = QLatin1String(defaultConnection));

    static bool removeDatabaseFiles(const QString &path, const QString &database);

    QEjdbDatabase(const QEjdbDatabase &other);

    QEjdbDatabase &operator =(const QEjdbDatabase &other);

    bool save(const QString &collectionName, QBsonObject &bson);
    QBsonObject load(const QString &collectionName, const QString &oid);

    bool remove(const QString &collectionName, const QString &oid);
    bool remove(const QString &collectionName, QBsonObject obj);

    bool removeCollection(const QString &collectionName);

    QList<QBsonObject> query(const QString &collectionName, const QBsonObject &query);
protected:
    /**
     * @brief QEJDBDatabase constructor
     * @param url
     * @param mode
     */
    QEjdbDatabase(QString url, int mode);


private:

    friend class QEjdbDatabasePrivate;
    QEjdbDatabasePrivate *d;

    QT_STATIC_CONST char *defaultConnection;


    /**
     * @brief loadCollections when db is opened the collections are loaded.
     */
    void loadCollections();


};

#endif // QEJDBDATABASE_H
