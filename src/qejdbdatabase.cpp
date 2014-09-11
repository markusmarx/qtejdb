#include "qejdbdatabase.h"
#include <QDebug>
#include <QDir>
#include <QReadLocker>
#include <QReadWriteLock>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>

#include "qatomic.h"
#include "qejdbcondition.h"
#include "ejdb.h"
#include "bson.h"
#include "globals_p.h"

/**
 * @brief QEJDB::convert2BsonEntry convert the variant value in a bson repräsentation and
 * append to the given bson.
 *
 * @param bson pointer to a bson structur
 * @param attr attribute name to append.
 * @param value variant value. Supported types are: bool, string, int, double, longlong
 */
void QEJDB::convert2BsonEntry(bson *bson, const char* attr, QVariant value)
{
    switch (value.type()) {
        case QVariant::String:
            bson_append_string(bson, attr, value.toString().toLatin1()); break;
        case QVariant::Int:
            bson_append_int(bson, attr, value.toInt()); break;
        case QVariant::Double:
            bson_append_double(bson, attr, value.toDouble()); break;
        case QVariant::LongLong:
            bson_append_long(bson, attr, value.toLongLong()); break;
        case QVariant::Bool:
            bson_append_bool(bson, attr, value.toBool()); break;
        default: break;
    }
}

/**
 * @brief QEJDB::convert2Query  convert a given condition object in a bson query repräsentation.
 *
 * @param bq pointer to a bson structure.
 * @param condition QEjdbContition to convert.
 */
void QEJDB::convert2Query(bson *bq, QEjdbCondition condition)
{

    if (condition.type() == QEjdbCondition::EQUALS) {
        bson_append_string(bq, condition.attribute().toLatin1(), condition.value().toString().toLatin1());
    } else {

        bson_append_start_object(bq, condition.attribute().toLatin1());

        switch (condition.type()) {
            case QEjdbCondition::BEGIN:
                QEJDB::convert2BsonEntry(bq, QStringLiteral("$begin").toLatin1(), condition.value());
                break;
            default: break;
        }
        bson_append_finish_object(bq);
    }
}

/**
 * @brief QEJDB::convert2QJsonValue convert a bson type in a QJsonValue type.
 *
 * @param bt bson_type enum
 * @param it iterator that points to the value.
 *
 * @return QJsonValue()
 */
QJsonValue QEJDB::convert2QJsonValue(bson_type bt, bson_iterator *it)
{

    bson_oid_t *oid;
    switch (bt) {
        case BSON_LONG:
            return QJsonValue((qint64) bson_iterator_long(it));
        case BSON_INT:
            return QJsonValue(bson_iterator_int(it));
        case BSON_DOUBLE:
            return QJsonValue(bson_iterator_double(it));
        case BSON_STRING:
        case BSON_SYMBOL:
            return QJsonValue(bson_iterator_string(it));
        case BSON_OID:
            char xoid[25];
            oid = bson_iterator_oid(it);
            bson_oid_to_string(oid, xoid);
            return QJsonValue(QString(xoid));
        case BSON_ARRAY:
        case BSON_OBJECT:
            bson_iterator sit;
            BSON_ITERATOR_SUBITERATOR(it, &sit);
            if (bt == BSON_OBJECT) {
                QJsonObject subObj;
                convert2QJson2(subObj, &sit);
                return QJsonValue(subObj);
            } else {
                QJsonArray subArr;
                convert2QJson2(subArr, &sit);
                return QJsonValue(subArr);
            }
    }
}

/**
 * @brief QEJDB::convert2QJson2 append values from a bson iterator to the array.
 *
 * @param obj QJsonArray
 * @param it iterator points to the array.
 */
void QEJDB::convert2QJson2(QJsonArray &obj, bson_iterator *it)
{
    bson_type bt;
    while ((bt = bson_iterator_next(it)) != BSON_EOO)  {
        obj.append(QJsonValue(convert2QJsonValue(bt, it)));
    }

}

/**
 * @brief QEJDB::convert2QJson2 append values from bson iterator to the object.
 *
 * @param obj QJsonObject
 * @param it iterator point to the object.
 */
void QEJDB::convert2QJson2(QJsonObject &obj, bson_iterator *it)
{
    bson_type bt;

    while ((bt = bson_iterator_next(it)) != BSON_EOO) {
        QString key = BSON_ITERATOR_KEY(it);
        obj.insert(key, convert2QJsonValue(bt, it));
    }
}

/**
 * @brief QEJDB::convert2QJson convert a bson to a QJsonObject. The converter uses the ejdb bson2json function.
 * So all datatypes should supported. Currently this is not fully tested.
 *
 * @param bson pointer to a bson structure.
 * @return converted QJsonObject.
 */
QJsonObject QEJDB::convert2QJson(bson *bson)
{
    QJsonObject obj;
    bson_iterator it;

    BSON_ITERATOR_FROM_BUFFER(&it, bson->data);

    convert2QJson2(obj, &it);

    return obj;
}


/**
 * @brief QEJDB::convert2Bson convert the given QJsonObject to a bson repräsentation.
 *
 * @param obj QJsonObject
 *
 * @return bson
 */
bson QEJDB::convert2Bson(QJsonObject obj)
{

    bson bsrec;
    bson_init(&bsrec);

    if (obj.contains("_id")) {

        bson_oid_t oid;
        bson_oid_from_string(&oid, obj.value("_id").toString().toLatin1());
        bson_append_oid(&bsrec, "_id", &oid);
    }


    QStringList keys = obj.keys();

    foreach(QString key, keys) {
        QJsonValue v = obj.value(key);
        QEJDB::convert2Bson2(key.toLatin1(), v, bsrec);
    }

    bson_finish(&bsrec);

    return bsrec;
}

/**
 * @brief convert2Bson2 recursive method to convert QJsonValue 2 bson record.
 *
 * @param attr  const *char attribute name.
 * @param value QJsonValue to convert.
 * @param bsrec pointer to the bson record.
 */
void QEJDB::convert2Bson2(const char* attr, QJsonValue value, bson &bsrec)
{    

    if (value.type() == QJsonValue::Object) {
        bson_append_start_object(&bsrec, attr);
        QJsonObject jsonObj = value.toObject();
        QStringList keys = jsonObj.keys();

        foreach(QString key, keys) {
            QJsonValue v = jsonObj.value(key);
            QEJDB::convert2Bson2(key.toLatin1(), v, bsrec);
        }

        bson_append_finish_object(&bsrec);
    } else if (value.type() ==QJsonValue::Array) {

       QJsonArray arr = value.toArray();
        QJsonArray::Iterator it;
        bson_append_start_array(&bsrec, attr);
        int i = 0;
        char kbuf[TCNUMBUFSIZ];
        for (it = arr.begin(); it != arr.end();++it) {
            bson_numstrn(kbuf, TCNUMBUFSIZ, i++);
            QEJDB::convert2Bson2(kbuf, *it, bsrec);
        }

        bson_append_finish_array(&bsrec);
    } else if (value.type() != QJsonValue::Undefined) {
        QEJDB::convert2BsonEntry(&bsrec, attr, value.toVariant());
    }

}

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
    QList<QJsonObject> query(QString collection, QEjdbCondition condition);
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

QList<QJsonObject> QEjdbDatabasePrivate::query(QString collectionName, QEjdbCondition condition)
{

    bson bq1;
    bson_init_as_query(&bq1);
    QEJDB::convert2Query(&bq1, condition);
    bson_finish(&bq1);

    EJCOLL *coll = ejdbgetcoll(m_db, collectionName.toLatin1());

    EJQ *q = ejdbcreatequery(m_db, &bq1, NULL, 0, NULL);

    uint32_t count;
    TCLIST *res = ejdbqryexecute(coll, q, &count, 0, NULL);

    QList<QJsonObject> resultList;

    for (int i = 0; i < TCLISTNUM(res); ++i) {
        void *bsdata = TCLISTVALPTR(res, i);
        int size = bson_size2(bsdata);
        bson *bs = bson_create_from_buffer(bsdata, size);

        QJsonObject obj = QEJDB::convert2QJson(bs);

        resultList.append(obj);

    }

    //Dispose result set
    tclistdel(res);

    //Dispose query
    ejdbquerydel(q);
    bson_destroy(&bq1);

    return resultList;

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


QEjdbDatabase::QEjdbDatabase(const QEjdbDatabase &other)
{
    d = other.d;
    d->ref.ref();
}


QEjdbDatabase &QEjdbDatabase::operator=(const QEjdbDatabase &other)
{
    qAtomicAssign<QEjdbDatabasePrivate>(d, other.d);
    return *this;
}

QList<QJsonObject> QEjdbDatabase::query(QString collection, QEjdbCondition condition)
{
    return d->query(collection, condition);
}

QEjdbDatabase::QEjdbDatabase()
{

}

QEjdbDatabase::~QEjdbDatabase()
{
    qAtomicDetach<QEjdbDatabasePrivate>(d);
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
