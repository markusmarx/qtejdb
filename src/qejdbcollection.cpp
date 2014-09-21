#include "qejdbcollection.h"
#include <QDebug>
#include "globals_p.h"
#include "ejdb.h"
#include "bson/qbsonobject.h"
#include "bson/qbsonobject_p.h"
#include "qejdbcondition.h"


class QEjdbCollectionPrivate
{
public:

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

    bool removeCollection();

    QList<QBsonObject> query(QEjdbCollectionPrivate* col, const QEjdbCondition& cond);

    static void convert2Query(bson *bq, const QEjdbCondition &condition);
    static void appendQueryValue(bson *bq, const QString& attr, const QVariant &v);
};



QEjdbCollection::QEjdbCollection(EJDB *db, EJCOLL *col, QString collectionName)
    :d(new QEjdbCollectionPrivate(this, db, col, collectionName))
{

}

void QEjdbCollectionPrivate::appendQueryValue(bson* bq, const QString& attr, const QVariant& v)
{
    switch (v.type()) {
        case QMetaType::QString:
            bson_append_string(bq, attr.toLatin1(), v.toString().toLatin1());
        break;
        case QMetaType::Int:
            bson_append_int(bq, attr.toLatin1(), v.toInt());
        break;
        case QMetaType::Double:
            bson_append_double(bq, attr.toLatin1(), v.toDouble());
        break;
        case QMetaType::Long:
            bson_append_long(bq, attr.toLatin1(), v.toLongLong());
        break;
        case QMetaType::Bool:
            bson_append_bool(bq, attr.toLatin1(), v.toBool());
        break;
        case QMetaType::QVariantList: {
            QList<QVariant> vl = v.toList();
            QList<QVariant>::Iterator it = vl.begin();
            int i = 0;
            bson_append_start_array(bq, attr.toLatin1());
            for (;it != vl.end();) {
                appendQueryValue(bq, QString::number(i), *it);
            }
            bson_append_finish_array(bq);
            break;
        }
    }

}


/**
 * @brief QEjdbCollectionPrivate::convert2Query  convert a given condition object in a bson query repräsentation.
 *
 * @param bq pointer to a bson structure.
 * @param condition QEjdbCondition to convert.
 */
void QEjdbCollectionPrivate::convert2Query(bson *bq, const QEjdbCondition &condition)
{

    if (condition.type() == QEjdbCondition::EQUALS) {
        appendQueryValue(bq, condition.attribute(), condition.value());
    } else {

        bson_append_start_object(bq, condition.attribute().toLatin1());

        switch (condition.type()) {
            case QEjdbCondition::BEGIN:
                appendQueryValue(bq, "$begin", condition.value());
                break;
            default: break;
        }
        bson_append_finish_object(bq);
    }
}

bool QEjdbCollectionPrivate::save(QBsonObject &obj)
{
    bson_oid_t oid;

    // get bson
    QBsonObjectData *d = obj.data;
    bson bsrec = QBsonObjectData::convert2Bson(*d);

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
    QBsonObject obj(bsrec);
    bson_del(bsrec);

    return obj;
}

bool QEjdbCollectionPrivate::removeCollection()
{
    if (!ejdbrmcoll(this->m_db, this->m_collectionName.toLatin1(), true)) {
        return false;
    }

    return true;
}

QList<QBsonObject> QEjdbCollectionPrivate::query(QEjdbCollectionPrivate* col, const QEjdbCondition &condition)
{
    bson bq1;
    bson_init_as_query(&bq1);

    QEjdbCollectionPrivate::convert2Query(&bq1, condition);

    bson_finish(&bq1);

    EJCOLL *coll = col->m_col;

    EJQ *q = ejdbcreatequery(col->m_db, &bq1, NULL, 0, NULL);

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

bool QEjdbCollection::removeCollection()
{
    return d->removeCollection();
}

QList<QBsonObject> QEjdbCollection::query(const QEjdbCondition &condition)
{
    return d->query(d, condition);
}

QEjdbCollection::~QEjdbCollection()
{
   if (!d->ref.deref())
       delete d;
}

