#include "qejdbcollection.h"
#include "ejdb.h"
#include <QDebug>
#include <QJsonDocument>

class QEjdbCollectionPrivate
{
public:

    QEjdbCollectionPrivate(QEjdbCollection *d, EJDB *db,  EJCOLL *col,  QString collectionName)
        :q(d),m_db(db), m_col(col), m_collectionName(collectionName)
    {

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

    /**
     * @brief convert2Bson convert the given jsonobject to the bson repräsentation
     * @param obj
     * @return
     */
    static bson convert2Bson(QJsonObject obj);

    /**
     * @brief convert2Bson2 recursive method to convert json 2 bson.
     * @param obj
     * @param bsrec
     */
    static void convert2Bson2(QJsonObject obj, bson &bsrec);



    bool save(QJsonObject &obj);
    QJsonObject load(QString oidStr);
};

bson QEjdbCollectionPrivate::convert2Bson(QJsonObject obj)
{

    bson bsrec;
    bson_init(&bsrec);

    if (obj.contains("_id")) {

        bson_oid_t oid;
        bson_oid_from_string(&oid, obj.value("_id").toString().toLatin1());
        bson_append_oid(&bsrec, "_id", &oid);
    }

    QEjdbCollectionPrivate::convert2Bson2(obj, bsrec);

    bson_finish(&bsrec);

    return bsrec;
}


void QEjdbCollectionPrivate::convert2Bson2(QJsonObject obj, bson &bsrec)
{

    QStringList keys = obj.keys();

    foreach(QString key, keys) {

        QJsonValue v = obj.value(key);

        switch (v.type()) {
            case QJsonValue::String:
                bson_append_string(&bsrec, key.toLatin1(), v.toString().toLatin1());break;
            case QJsonValue::Bool:
                bson_append_bool(&bsrec, key.toLatin1(), v.toBool());break;
            case QJsonValue::Double:
                bson_append_double(&bsrec, key.toLatin1(), v.toDouble());break;
            case QJsonValue::Object:
                bson_append_start_object(&bsrec, key.toLatin1());
                QJsonObject jsonObj = v.toObject();
                QEjdbCollectionPrivate::convert2Bson2(jsonObj, bsrec);
                bson_append_finish_object(&bsrec);
                break;
        }

    }

}



QEjdbCollection::QEjdbCollection(EJDB *db, EJCOLL *col, QString collectionName)
    :d(new QEjdbCollectionPrivate(this, db, col, collectionName))
{

}


bool QEjdbCollectionPrivate::save(QJsonObject &obj)
{
    bson_oid_t oid;

    // get bson
    bson bsrec = QEjdbCollectionPrivate::convert2Bson(obj);

    // save
    bool res = ejdbsavebson(m_col, &bsrec, &oid);
    char oidhex[25];
    bson_oid_to_string(&oid, oidhex);
    obj.insert("_id", QJsonValue(QString(oidhex)));
    bson_destroy(&bsrec);

    return res;
}

QJsonObject QEjdbCollectionPrivate::load(QString oidStr)
{

    bson_oid_t oid;
    bson_oid_t *oid_t;
    bson_oid_from_string(&oid, oidStr.toLatin1());

    bson* bsrec = ejdbloadbson(m_col, &oid);
    char* buf;
    int length;

    bson2json(bsrec->data, &buf, &length);

    QJsonDocument doc = QJsonDocument::fromJson(QByteArray(buf, length));

    //bson_print_raw(bsrec->data, 0);

    QJsonObject obj = doc.object();

    /*bson_iterator *it = bson_iterator_create();
    bson_iterator_init(it, bsrec);


    bson_type bstype;
    while (bson_iterator_more(it)) {
        bstype = bson_iterator_next(it);
        const char* key = bson_iterator_key(it);


        switch(bstype) {
            case BSON_OID:

                oid_t = bson_iterator_oid(it);
                char oidhex[25];
                bson_oid_to_string(oid_t, oidhex);
                obj.insert("_id", QJsonValue(QString(oidhex)));
                break;
            case BSON_STRING:
                obj.insert(QString(key), QJsonValue(QString(bson_iterator_string(it))));
                break;
            case BSON_DOUBLE:
                obj.insert(QString(key), QJsonValue(bson_iterator_double(it)));
                break;
        }

    }
    bson_iterator_dispose(it);*/
    return obj;
}




bool QEjdbCollection::save(QJsonObject &obj)
{
    return d->save(obj);
}

QJsonObject QEjdbCollection::load(QString oid)
{
    return d->load(oid);
}

QString QEjdbCollection::collectionName() const
{
    return d->m_collectionName;
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

QEjdbCollection::~QEjdbCollection()
{
    qAtomicDetach<QEjdbCollectionPrivate>(d);
}

