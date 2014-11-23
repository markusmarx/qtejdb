#include "qejdbdatabase.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QReadLocker>
#include <QReadWriteLock>
#include "bson/qbsonobject.h"
#include "qejdbquery.h"


#include "qatomic.h"
#include "ejdb.h"
#include "bson.h"
#include "qejdbworker.h"
#include "bson/qbsonobject.h"


/**
 * Hash to store open connections. Copied from QSqlDatabase.
 *
 * @brief The QEjdbConnectionDict class
 */
class QEjdbConnectionDict: public QHash<QString, QEjdbDatabase>
{
public:
    inline bool contains_ts(const QString &key)
    {
        QReadLocker locker(&lock);
        return contains(key);
    }
inline QStringList keys_ts() const
    {
        QReadLocker locker(&lock);
        return keys();
    }

    mutable QReadWriteLock lock;
};

Q_GLOBAL_STATIC(QEjdbConnectionDict, ejdbDict)


class QEjdbDatabasePrivate {
public:
    QAtomicInt ref;
    QEjdbWorker *m_worker;
    QEjdbDatabasePrivate(QEjdbDatabase *d, const QUrl &url, int mode):
        q(d), m_worker(QEjdbWorker::createFromUrl(url, mode))
    {
        ref = 1;

    }

    ~QEjdbDatabasePrivate()
    {
        delete m_worker;
    }


    void open();
    bool close();
    bool isOpen() const;
    QEjdbCollection collection(QString collectionName);
    QEjdbCollection storeCollection(EJCOLL *col, QString collectionName);
    bool createCollection(const QString &collectionName);
    bool removeCollection(const QString &collectionName);
    bool containsCollection(const QString &collectionName);

    static void removeDatabase(const QString &name);
    static void addDatabase(const QEjdbDatabase &db, const QString &name);
    static QEjdbDatabase database(const QString &name, bool open);
private:

    /**
     * @brief q
     */
    QEjdbDatabase *q;

    /**
     * @brief m_dbPath contains the path to the db files.
     */
    QDir m_dbPath;

    /**
     * @brief m_dbName contains the db name.
     */
    QString m_dbName;

    /**
     * @brief m_mode database open mode
     */
    int m_mode;

    /**
     * @brief m_collections
     */
    QHash<QString, QEjdbCollection> m_collections;

    inline void checkConnection() const
    {
        if (!isOpen()) {
            throw QEjdbException(QEjdbException::NOTCONNECTED, "database is not connected.");
        }
    }

};

void QEjdbDatabasePrivate::open()
{

    if (isOpen()) return;
    m_worker->open();
}

bool QEjdbDatabasePrivate::close()
{
    return m_worker->close();
}

bool QEjdbDatabasePrivate::isOpen() const
{
    return m_worker->isOpen();
}

bool QEjdbDatabasePrivate::containsCollection(const QString &collectionName)
{
    checkConnection();
    return m_worker->containsCollection(collectionName);
}


bool QEjdbDatabasePrivate::createCollection(const QString &collectionName)
{
    checkConnection();
    return m_worker->createCollection(collectionName);
}

bool QEjdbDatabasePrivate::removeCollection(const QString &collectionName)
{
    checkConnection();
    return m_worker->removeCollection(collectionName);
}

void QEjdbDatabasePrivate::removeDatabase(const QString &name)
{
    QEjdbConnectionDict *dict = ejdbDict();
    Q_ASSERT(dict);
    QWriteLocker locker(&dict->lock);

    if (!dict->contains(name))
        return;

    dict->take(name).close();

}

void QEjdbDatabasePrivate::addDatabase(const QEjdbDatabase &db, const QString &name)
{
    QEjdbConnectionDict *dict = ejdbDict();
    Q_ASSERT(dict);
    QWriteLocker locker(&dict->lock);

    if (dict->contains(name)) {
        dict->take(name).close();

    }
    dict->insert(name, db);
}


QEjdbDatabase QEjdbDatabasePrivate::database(const QString& name, bool open)
{
    const QEjdbConnectionDict *dict = ejdbDict();
    Q_ASSERT(dict);

    dict->lock.lockForRead();
    QEjdbDatabase db = dict->value(name);
    dict->lock.unlock();
    if (!db.isOpen() && open) {
        db.open();
    }
    return db;
}


QT_STATIC_CONST_IMPL char *QEjdbDatabase::defaultConnection = "qejdb_default_connection";

QEjdbDatabase::QEjdbDatabase(QString url, int mode):d(new QEjdbDatabasePrivate(this, QUrl(url), mode))
{
}

QEjdbDatabase QEjdbDatabase::addDatabase(QString url, int mode, QString connectionName)
{
    QEjdbDatabase db(url, mode);
    QEjdbDatabasePrivate::addDatabase(db, connectionName);
    return db;
}

void QEjdbDatabase::removeDatabase(const QString &connectionName)
{
    QEjdbDatabasePrivate::removeDatabase(connectionName);
}

bool QEjdbDatabase::removeDatabaseFiles(const QString &path, const QString &database)
{
    // set filter for databasefiles and remove
    QStringList filter;
    filter << database + "*";
    QDir dbPath(path);
    dbPath.setNameFilters(filter);
    QFileInfoList files = dbPath.entryInfoList(QDir::Files);

    foreach(QFileInfo f, files) {
        if (f.isFile()) {
            QFile(f.absoluteFilePath()).remove();
        }
    }
    return true;
}

QEjdbDatabase QEjdbDatabase::database(const QString &connectionName)
{
    return QEjdbDatabasePrivate::database(connectionName, true);
}


QEjdbDatabase::QEjdbDatabase(const QEjdbDatabase &other):d(other.d)
{
    d->ref.ref();
}


QEjdbDatabase &QEjdbDatabase::operator=(const QEjdbDatabase &other)
{
    if (d) {
        qAtomicAssign<QEjdbDatabasePrivate>(d, other.d);
    } else {
        d = other.d;
        d->ref.ref();
    }
    return *this;
}

bool QEjdbDatabase::save(const QString &collectionName, QBsonObject &bson)
{
    return d->m_worker->save(collectionName, bson);
}

QBsonObject QEjdbDatabase::load(const QString &collectionName, const QString &oid)
{
    return d->m_worker->load(collectionName, oid);
}

bool QEjdbDatabase::remove(const QString &collectionName, const QString &oid)
{
    return d->m_worker->remove(collectionName, oid);
}

bool QEjdbDatabase::remove(const QString &collectionName, QBsonObject obj)
{
    if (!obj.contains("_id") || !obj.value("_id").isValid()) return false;
    QString oid = obj.value("_id").toString();
    return remove(collectionName, oid);
}

QList<QBsonObject> QEjdbDatabase::query(const QString &collectionName, const QBsonObject& query)
{
    return d->m_worker->query(collectionName, query);
}

QEjdbDatabase::QEjdbDatabase():d(0)
{

}

QEjdbDatabase::~QEjdbDatabase()
{
    if (!d->ref.deref()) {
        delete d;
        d = 0;
    }
}

void QEjdbDatabase::open()
{
   return d->open();
}

bool QEjdbDatabase::close()
{
    return d->close();
}

bool QEjdbDatabase::isOpen()
{
    return d->isOpen();
}

bool QEjdbDatabase::containsCollection(const QString &collectionName)
{
    return d->containsCollection(collectionName);
}


bool QEjdbDatabase::createCollection(const QString &collectionName)
{
    return d->createCollection(collectionName);
}

bool QEjdbDatabase::removeCollection(const QString& collectionName )
{
    return d->removeCollection(collectionName);
}
