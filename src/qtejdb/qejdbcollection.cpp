#include "qejdbcollection.h"
#include "qbsonfrombsonrec.h"
#include "qbsontobsonrec.h"
#include <QDebug>
#include "ejdb.h"
#include "qbsonobject.h"
#include "qbsonobject_p.h"
#include "qejdbquery.h"


class QEjdbCollectionPrivate
{
public:
    
    /**
     * @brief 
     * @details [long description]
     * 
     * @param d [description]
     * @param db [description]
     * @param col [description]
     * @param collectionName [description]
     * @param b [description]
     * @param l [description]
     * @param e [description]
     */
    QEjdbCollectionPrivate(QEjdbCollection *d, EJDB *db,  EJCOLL *col,  QString collectionName)
        :q(d),m_db(db), m_col(col), m_collectionName(collectionName)
    {
        ref = 1;
    }

    QEjdbCollection *q;

    /**
     * @brief m_db reference to database
     */
    EJDB* m_db;

    /**
     * @brief m_col reference to collection structure.
     */
    EJCOLL* m_col;

    /**
     * @brief m_collectionName columnname.
     */
    QString m_collectionName;

    QAtomicInt ref;


    bool save(QBsonObject &obj);
    bool remove(QBsonObject &obj);

    QBsonObject load(QString oidStr);

    QList<QBsonObject> query(const QBsonObject& cond);

};



QEjdbCollection::QEjdbCollection(EJDB *db, EJCOLL *col, QString collectionName)
    :d(new QEjdbCollectionPrivate(this, db, col, collectionName))
{

}


bool QEjdbCollectionPrivate::save(QBsonObject &obj)
{
    bson_oid_t oid;

    // get bson
    QBsonObjectData *d = obj.data;
    bson bsrec = convertQBsonToBsonRec(d, 0);

    // save
    bool res = ejdbsavebson(m_col, &bsrec, &oid);
    char oidhex[25];
    bson_oid_to_string(&oid, oidhex);
    obj.insert("_id", QBsonOid(oidhex));
    bson_destroy(&bsrec);

    return res;
}

bool QEjdbCollectionPrivate::remove(QBsonObject &obj)
{
    QString oidStr = obj.value("_id").toString();
    bson_oid_t oid;

    bson_oid_from_string(&oid, oidStr.toLatin1());
    return ejdbrmbson(m_col, &oid);
}

QBsonObject QEjdbCollectionPrivate::load(QString oidStr)
{

    bson_oid_t oid;
    bson_oid_from_string(&oid, oidStr.toLatin1());

    bson* bsrec = ejdbloadbson(m_col, &oid);
    QBsonObject obj;
    convertBsonObjectFromBsonRec(bsrec, obj);

    bson_del(bsrec);

    return obj;
}


QList<QBsonObject> QEjdbCollectionPrivate::query(const QBsonObject &query)
{
    bson bq1;
    bson_init_as_query(&bq1);
    QBsonObjectData *d = query.data;
    bq1 = convertQBsonToBsonRec(d, &bq1);

    bson_finish(&bq1);

    EJCOLL *coll = this->m_col;

    EJQ *q = ejdbcreatequery(this->m_db, &bq1, NULL, 0, NULL);

    uint32_t count;
    TCLIST *res = ejdbqryexecute(coll, q, &count, 0, NULL);

    QList<QBsonObject> resultList;

    for (int i = 0; i < TCLISTNUM(res); ++i) {
        void *bsdata = TCLISTVALPTR(res, i);
        int size = bson_size2(bsdata);
        bson *bs = bson_create_from_buffer(bsdata, size);
        QBsonObject obj;
        convertBsonObjectFromBsonRec(bs, obj);

        resultList.append(obj);
        bson_del(bs);

    }

    //Dispose result set
    tclistdel(res);

    //Dispose query
    ejdbquerydel(q);
    bson_destroy(&bq1);


    return resultList;

}

bool QEjdbCollection::save(QBsonObject &obj)
{
    return d->save(obj);
}

QBsonObject QEjdbCollection::load(QString oid)
{
    return d->load(oid);
}

bool QEjdbCollection::remove(QBsonObject &obj)
{
    return d->remove(obj);
}

QString QEjdbCollection::collectionName() const
{
    return QString(d->m_collectionName);
}

QEjdbCollection::QEjdbCollection(const QEjdbCollection &other)
{
    d = other.d;
    d->ref.ref();
}


QEjdbCollection &QEjdbCollection::operator=(const QEjdbCollection &other)
{
    qAtomicAssign<QEjdbCollectionPrivate>(d, other.d);
    return *this;
}


QList<QBsonObject> QEjdbCollection::query(const QBsonObject &query)
{
    return d->query(query);
}

bool QEjdbCollection::isValid() const
{
    return d->m_col != 0;
}

QEjdbCollection::~QEjdbCollection()
{
   if (!d->ref.deref()) {
       delete d;
       d = 0;
   }

}

QEjdbCollection::QEjdbCollection(const QString& collectionName):
    d(new QEjdbCollectionPrivate(this, 0, 0, collectionName))
{

}

