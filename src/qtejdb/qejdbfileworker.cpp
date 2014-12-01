#include "qejdbworker.h"
#include "qbsonobject_p.h"
#include "qejdbexception.h"
#include "qejdbresult.h"
#include "qejdbresult_p.h"

QEjdbFileWorker::QEjdbFileWorker(const QUrl &url, int mode):
    m_path(url.toLocalFile()), m_mode(mode), m_db(0)
{
}

void QEjdbFileWorker::open()
{
    m_db = ejdbnew();

    if (!ejdbopen(m_db, m_path.toLatin1(), m_mode)) {
        throw QEjdbException(QEjdbException::CONNECTIONERROR, ejdberrmsg(ejdbecode(m_db)));
    }

}

bool QEjdbFileWorker::isOpen()
{
    return m_db && ejdbisopen(m_db);
}

bool QEjdbFileWorker::close()
{
    bool res;

    if (!isOpen()) return false;

    if (!ejdbclose(m_db)) res = false;
    ejdbdel(m_db);
    return res;
}

bool QEjdbFileWorker::containsCollection(const QString &collectionName)
{
    if (!isOpen()) return false;

    EJCOLL *col = ejdbgetcoll(m_db, collectionName.toLatin1());
    return col != 0;
}

bool QEjdbFileWorker::createCollection(const QString &collectionName)
{
    if (containsCollection(collectionName)) return false;
    EJCOLLOPTS opts;
    opts.compressed = true;
    opts.large = true;
    opts.records = 1280000;
    EJCOLL *col = ejdbcreatecoll(m_db, collectionName.toLatin1(), &opts);
    Q_UNUSED(col);
    return col!=0;
}

bool QEjdbFileWorker::removeCollection(const QString &collectionName)
{
    if (containsCollection(collectionName)) {
        if (!ejdbrmcoll(this->m_db, collectionName.toLatin1(), true)) {
            return false;
        }
        m_collectionDict.remove(collectionName);
        return true;
    }
    return false;
}

bool QEjdbFileWorker::save(const QString &collectionName, QBsonObject &bsonObj)
{
    bson_oid_t oid;

    // get bson
    QBsonObjectData *d = bsonObj.data;
    bson bsrec = QBsonObjectData::convert2Bson(*d);
    EJCOLL *col = getCollection(collectionName);
    // save
    bool res = ejdbsavebson(col, &bsrec, &oid);
    if (!bsonObj.contains("_id")) {
        char oidhex[25];
        bson_oid_to_string(&oid, oidhex);
        bsonObj.insert("_id", QBsonOid(oidhex));
    }
    bson_destroy(&bsrec);
    return res;
}

QBsonObject QEjdbFileWorker::load(const QString &collectionName, const QString &oidStr)
{
    bson_oid_t oid;
    bson_oid_from_string(&oid, oidStr.toLatin1());
    EJCOLL *col = getCollection(collectionName);
    bson* bsrec = ejdbloadbson(col, &oid);
    QBsonObject obj(bsrec);
    bson_del(bsrec);

    return obj;
}

bool QEjdbFileWorker::remove(const QString &collectionName, const QString &oidStr)
{
    bson_oid_t oid;
    EJCOLL *col = getCollection(collectionName);
    bson_oid_from_string(&oid, oidStr.toLatin1());
    return ejdbrmbson(col, &oid);
}

QEjdbResult QEjdbFileWorker::query(const QString &collectionName, const QBsonObject &query)
{
    bson bq1;
    bson_init_as_query(&bq1);
    QBsonObjectData *d = query.data;
    bq1 = QBsonObjectData::convert2Bson(*d, &bq1);

    bson_finish(&bq1);

    EJCOLL *coll = getCollection(collectionName);

    EJQ *q = ejdbcreatequery(this->m_db, &bq1, NULL, 0, NULL);

    uint32_t count;
    TCLIST *res = ejdbqryexecute(coll, q, &count, 0, NULL);

    QList<QBsonObject> resultList;

    for (int i = 0; i < TCLISTNUM(res); ++i) {
        void *bsdata = TCLISTVALPTR(res, i);
        int size = bson_size2(bsdata);
        bson *bs = bson_create_from_buffer(bsdata, size);
        QBsonObject obj;
        QBsonObjectData::convert2QBson(bs, obj);

        resultList.append(obj);
        bson_del(bs);

    }

    //Dispose result set
    tclistdel(res);

    //Dispose query
    ejdbquerydel(q);
    bson_destroy(&bq1);

    QEjdbResult result;

    result.d()->values = resultList;

    return result;

}

int QEjdbFileWorker::mode() const
{
    return m_mode;
}

EJCOLL *QEjdbFileWorker::getCollection(const QString &collectionName)
{
    if (m_collectionDict.contains(collectionName)) {
        return m_collectionDict.value(collectionName);
    }
    EJCOLL *col = ejdbgetcoll(m_db, collectionName.toLatin1());
    m_collectionDict.insert(collectionName, col);
    return col;
}
