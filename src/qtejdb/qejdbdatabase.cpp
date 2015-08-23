#include "qejdbdatabase.h"
#include "qejdbdatabase_p.h"
#include "qejdbcollection_p.h"
#include "qejdbquery.h"

/*!
  \class QEjdbDatabase
  \brief QEjdbDatabase connects a ejdb database and provides an api to modify
  it.

  \ingroup qtejdb-database
  \inmodule qtejdb-database
 */

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
    QEjdbDatabasePrivate::addDatabase(db, connectionName);
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

    d = other.d;
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

QEjdbCollection QEjdbDatabase::collection(const QString &collection, bool create)
{
    Q_ASSERT(d);
    QSharedPointer<QEjdbCollectionPrivate> col;
    if (d->m_collections.contains(collection)) {
        col = d->m_collections.value(collection);
    } else {
        col = QSharedPointer<QEjdbCollectionPrivate>(
                    new QEjdbCollectionPrivate(collection, d)
                    );
        d->m_collections.insert(collection, col);
    }

    if (create && !col->exist()) {
        col->create();
    }

    return QEjdbCollection(col);
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

bool QEjdbDatabase::dropCollection(const QString& collectionName )
{
    Q_ASSERT(d);
    return d->removeCollection(collectionName);
}
