#include "qejdbdatabase.h"
#include <QDebug>
#include <QDir>
#include <QReadLocker>
#include <QReadWriteLock>
#include "bson/qbsonobject.h"

#include "qatomic.h"
#include "qejdbcondition.h"
#include "ejdb.h"
#include "bson.h"


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
    QEjdbDatabasePrivate(QEjdbDatabase *d, QString path, QString database, int mode):
        q(d), m_dbPath(QDir(path)), m_dbName(database), m_db(0), m_mode(mode){
        ref = 1;
    }
    bool open();
    bool close();
    bool isOpen();
    QEjdbCollection collection(QString collectionName);
    QEjdbCollection storeCollection(EJCOLL *col, QString collectionName);
    QEjdbCollection createCollection(QString collectionName);
    bool containsCollection(QString collectionName);
    QList<QBsonObject> query(QString collection, QEjdbCondition condition);
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
     * @brief m_db pointer to ejdb.
     */
    EJDB* m_db;

    /**
     * @brief m_mode database open mode
     */
    int m_mode;

    /**
     * @brief m_collections
     */
    QHash<QString, QSharedPointer<QEjdbCollection> > m_collections;

};

bool QEjdbDatabasePrivate::open() {

    if (isOpen()) close();

    m_db = ejdbnew();

    bool res = ejdbopen(m_db, m_dbPath.absoluteFilePath(m_dbName).toLatin1(), m_mode);

    if (!res) return false;
    return true;
}

bool QEjdbDatabasePrivate::close()
{
    bool res;

    if (!isOpen()) return false;

    if (!ejdbclose(m_db)) res = false;
    ejdbdel(m_db);

    return res;
}

bool QEjdbDatabasePrivate::isOpen()
{
    return m_db && ejdbisopen(m_db);
}


bool QEjdbDatabasePrivate::containsCollection(QString collectionName)
{
    EJCOLL *col = ejdbgetcoll(m_db, collectionName.toLatin1());

    return col != 0;
}

QList<QBsonObject> QEjdbDatabasePrivate::query(QString collectionName, QEjdbCondition condition)
{

    QEjdbCollection col = collection(collectionName);

    return col.query(condition);
}

QEjdbCollection QEjdbDatabasePrivate::collection(QString collectionName)
{
    if (m_collections.contains(collectionName)) {
        QSharedPointer<QEjdbCollection> ptr = m_collections.value(collectionName);
        return *ptr;
    }

    EJCOLL *col = ejdbgetcoll(m_db, collectionName.toLatin1());

    return storeCollection(col, collectionName);
}

QEjdbCollection QEjdbDatabasePrivate::createCollection(QString collectionName)
{
    EJCOLL *col = ejdbcreatecoll(m_db, collectionName.toLatin1(), NULL);
    return storeCollection(col, collectionName);
}

QEjdbCollection QEjdbDatabasePrivate::storeCollection(EJCOLL *col, QString collectionName)
{
    QSharedPointer<QEjdbCollection> ptr;
    if (m_collections.contains(collectionName)) {
        //todo exception handling

    } else{
        ptr = QSharedPointer<QEjdbCollection>(new QEjdbCollection(m_db, col, collectionName));
        m_collections.insert(collectionName, ptr);
    }
    return *ptr;
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
    //db.d->connName = name;
}

QEjdbDatabase QEjdbDatabasePrivate::database(const QString& name, bool open)
{
    const QEjdbConnectionDict *dict = ejdbDict();
    Q_ASSERT(dict);

    dict->lock.lockForRead();
    QEjdbDatabase db = dict->value(name);
    dict->lock.unlock();
    if (!db.isOpen() && open) {
        if (!db.open())
            qWarning() << "QEjdbDatabasePrivate::database: unable to open database";

    }
    return db;
}


QT_STATIC_CONST_IMPL char *QEjdbDatabase::defaultConnection = "qejdb_default_connection";

QEjdbDatabase::QEjdbDatabase(QString path, QString database, int mode):d(new QEjdbDatabasePrivate(this, path, database, mode))
{
}

QEjdbDatabase QEjdbDatabase::addDatabase(QString path, QString database, int mode, QString connectionName)
{
    QEjdbDatabase db(path, database, mode);
    QEjdbDatabasePrivate::addDatabase(db, connectionName);
    return db;
}

void QEjdbDatabase::removeDatabase(QString connectionName)
{
    QEjdbDatabasePrivate::removeDatabase(connectionName);
}

bool QEjdbDatabase::removeDatabaseFiles(QString path, QString database)
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

QEjdbDatabase QEjdbDatabase::database(QString connectionName)
{
    return QEjdbDatabasePrivate::database(connectionName, true);
}


QEjdbDatabase::QEjdbDatabase(const QEjdbDatabase &other):d(other.d)
{
    d->ref.ref();
}


QEjdbDatabase &QEjdbDatabase::operator=(const QEjdbDatabase &other)
{
    qAtomicAssign<QEjdbDatabasePrivate>(d, other.d);
    return *this;
}

QList<QBsonObject> QEjdbDatabase::query(QString collection, QEjdbCondition condition)
{
    return d->query(collection, condition);
}

QEjdbDatabase::QEjdbDatabase()
{

}

QEjdbDatabase::~QEjdbDatabase()
{
    if (!d->ref.deref())
        delete d;
}

bool QEjdbDatabase::open()
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


QEjdbCollection QEjdbDatabase::collection(QString collectionName)
{
    if (!d->containsCollection(collectionName)) {
        throw 1;
    }

    return d->collection(collectionName);
}

QEjdbCollection QEjdbDatabase::createCollection(QString collectionName)
{
    return d->createCollection(collectionName);
}
