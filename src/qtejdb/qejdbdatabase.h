#ifndef QEJDBDATABASE_H
#define QEJDBDATABASE_H
#include <QString>
#include <QSharedPointer>
#include <QDir>
#include <QUrl>
#include "qejdbexception.h"
#include "qejdbresult.h"
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
        SYNC        = 1 << 6,
        DEFAULT     =  CREATE | WRITE
                       | LOCK_NB | TRUNCATE
    };

}

QT_FORWARD_DECLARE_CLASS(QEjdbDatabasePrivate)
QT_FORWARD_DECLARE_CLASS(QEjdbCollectionPrivate)
QT_FORWARD_DECLARE_CLASS(QEjdbQuery)


class Q_DECL_EXPORT QEjdbDatabase
{

public:

    QEjdbDatabase();

    /**
     * @brief ~QEJDBDatabase destructor
     */
    ~QEjdbDatabase();


    /**
     * @brief createCollection
     * @param collectionName
     * @return
     */
    bool createCollection(const QString &collectionName);

    bool containsCollection(const QString &collectionName);

    /**
     * @brief addDatabase Add a database connection to the pool.
     *
     * @param url
     * @param mode
     * @param connectionName
     * @return
     */
    static QEjdbDatabase addDatabase(QString url, int mode, QString connectionName = QLatin1String(defaultConnection));
    /**
     * @brief addDatabase
     * @param url
     * @param connectionName
     * @return
     */
    static QEjdbDatabase addDatabase(QString url, QString connectionName = QLatin1String(defaultConnection));
    /**
     * @brief database
     * @param connectionName
     * @return
     */
    static QEjdbDatabase database(const QString &connectionName = QLatin1String(defaultConnection));
    static void removeDatabase(const QString &connectionName  = QLatin1String(defaultConnection));
    static bool removeDatabaseFiles(const QString &path, const QString &database);
    QEjdbDatabase(const QEjdbDatabase &other);
    QEjdbDatabase &operator =(const QEjdbDatabase &other);
    bool save(const QString &collectionName, QBsonObject &bson);
    QBsonObject load(const QString &collectionName, const QString &oid);
    QEjdbResult loadAll(const QString &collectionName);
    bool remove(const QString &collectionName, const QString &oid);
    bool remove(const QString &collectionName, QBsonObject obj);
    bool dropCollection(const QString &collectionName);

    QEjdbResult query(const QString &collectionName, const QBsonObject &query, const QBsonObject& hints = QBsonObject());
    QString connectionName() const;
    void open();
    bool close();
    bool isOpen();

    QEjdbCollection collection(const QString &collection, bool create = false);
protected:
    /**
     * @brief QEJDBDatabase constructor
     * @param url
     * @param mode
     */
    QEjdbDatabase(QString url, int mode, const QString &connectionName);

private:

    friend class QEjdbDatabasePrivate;
    QSharedPointer<QEjdbDatabasePrivate> d;

    static const char *defaultConnection;

    /**
     * @brief loadCollections when db is opened the collections are loaded.
     */
    void loadCollections();


};


#endif // QEJDBDATABASE_H
