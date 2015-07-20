#include "qejdbdatabase.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QReadLocker>
#include <QReadWriteLock>
#include "qbson/qbsonobject.h"
#include "qejdbquery.h"

#include "qatomic.h"
#include "ejdb.h"
#include "bson.h"
#include "qejdbworker.h"
#include "qbson/qbsonobject.h"


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
    QEjdbDatabasePrivate(
            QEjdbDatabase *d, const QUrl &url, int mode, const QString &connectionName
    ):q(d), m_worker(QEjdbWorker::createFromUrl(url, mode))
      ,m_connectionName(connectionName)
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

    bool createCollection(const QString &collectionName);
    bool removeCollection(const QString &collectionName);
    bool containsCollection(const QString &collectionName);

    QString connectionName() const {
        return m_connectionName;
    }

    static void removeDatabase(const QString &name);
    static QEjdbDatabase addDatabase(const QEjdbDatabase &db, const QString &name);
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
     * @brief m_connectionName name of the connection stored in dictionary.
     */
    QString m_connectionName;

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

QEjdbDatabase QEjdbDatabasePrivate::addDatabase(const QEjdbDatabase &db, const QString &name)
{
    QEjdbConnectionDict *dict = ejdbDict();
    Q_ASSERT(dict);
    QWriteLocker locker(&dict->lock);

    if (dict->contains(name)) {
        return dict->value(name);

    }
    dict->insert(name, db);
    return db;
}


QEjdbDatabase QEjdbDatabasePrivate::database(const QString& name, bool open)
{
    const QEjdbConnectionDict *dict = ejdbDict();
    Q_ASSERT(dict);

    dict->lock.lockForRead();
    if (dict->contains(name)) {
        QEjdbDatabase db = dict->value(name);
        if (!db.isOpen() && open) {
            db.open();
        }
        dict->lock.unlock();
        return db;
    }
    dict->lock.unlock();
    qWarning() << "no database was found under key " + name;
    throw QEjdbException(QEjdbException::DBKEYNOTEXIST,
                         "no database was found under key " + name);

}


const char *QEjdbDatabase::defaultConnection = "qejdb_default_connection";

/**
 * @class QEjdbDatabase
 * @brief QEjdbDatabase
 */

QEjdbDatabase::QEjdbDatabase(
        QString url, int mode, const QString &connectionName
):d(new QEjdbDatabasePrivate(this, QUrl(url), mode, connectionName))
{
}

/**
 * @brief QEjdbDatabase::addDatabase
 * @param url
 * @param mode
 * @param connectionName
 * @return
 */
QEjdbDatabase QEjdbDatabase::addDatabase(QString url, int mode, QString connectionName)
{
    QEjdbDatabase db(url, mode, connectionName);
    db = QEjdbDatabasePrivate::addDatabase(db, connectionName);
    db.open();
    return db;
}

/**
 * @brief QEjdbDatabasePrivate::addDatabase
 * @param db
 * @param name
 */
QEjdbDatabase QEjdbDatabase::addDatabase(QString url, QString connectionName)
{
    QEjdbDatabase db(url, 0, connectionName);
    QEjdbDatabasePrivate::addDatabase(db, connectionName);
    db.open();
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

/**
 * @brief QEjdbDatabase::connectionName Returns connection name stored with
 * addDatabase
 *
 * @return connectonName
 */
QString QEjdbDatabase::connectionName() const
{
    Q_ASSERT(d);
    return d->connectionName();
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
    Q_ASSERT(d);
    return d->m_worker->save(collectionName, bson);
}

QBsonObject QEjdbDatabase::load(const QString &collectionName, const QString &oid)
{
    Q_ASSERT(d);
    return d->m_worker->load(collectionName, oid);
}

QEjdbResult QEjdbDatabase::loadAll(const QString &collectionName)
{
    Q_ASSERT(d);
    return d->m_worker->loadAll(collectionName);
}

bool QEjdbDatabase::remove(const QString &collectionName, const QString &oid)
{
    Q_ASSERT(d);
    return d->m_worker->remove(collectionName, oid);
}

bool QEjdbDatabase::remove(const QString &collectionName, QBsonObject obj)
{
    if (!obj.hasOid()) return false;
    return remove(collectionName, obj.oid());
}

QEjdbResult QEjdbDatabase::query(const QString &collectionName, const QBsonObject& query, const QBsonObject& hints)
{
    Q_ASSERT(d);
    return d->m_worker->query(collectionName, query, hints);
}

QEjdbDatabase::QEjdbDatabase():d(0)
{

}

QEjdbDatabase::~QEjdbDatabase()
{
    if (d && !d->ref.deref()) {
        delete d;
        d = 0;
    }
}

void QEjdbDatabase::open()
{
    Q_ASSERT(d);
    d->open();
}

bool QEjdbDatabase::close()
{
    Q_ASSERT(d);
    return d->close();
}

bool QEjdbDatabase::isOpen()
{
    Q_ASSERT(d);
    return d->isOpen();
}

bool QEjdbDatabase::containsCollection(const QString &collectionName)
{
    Q_ASSERT(d);
    return d->containsCollection(collectionName);
}


bool QEjdbDatabase::createCollection(const QString &collectionName)
{
    Q_ASSERT(d);
    return d->createCollection(collectionName);
}

bool QEjdbDatabase::removeCollection(const QString& collectionName )
{
    Q_ASSERT(d);
    return d->removeCollection(collectionName);
}
