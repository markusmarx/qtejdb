#include "qejdbcollection.h"
#include "qejdbdatabase.h"
#include <QDebug>

QEJDBCollection::QEJDBCollection(EJDB *db, EJCOLL *col, QString collectionName):m_db(db), m_col(col),
    m_collectionName(collectionName)
{


}


bool QEJDBCollection::save(QJsonObject &obj)
{
    bson_oid_t oid;

    // get bson
    bson bsrec = QEJDBCollection::convert2Bson(obj);

    // save
    bool res = ejdbsavebson(m_col, &bsrec, &oid);
    char oidhex[25];
    bson_oid_to_string(&oid, oidhex);
    obj.insert("_id", QJsonValue(QString(oidhex)));
    bson_destroy(&bsrec);

    return res;
}

QJsonObject QEJDBCollection::load(QString oidStr)
{
    qDebug() << oidStr;
    bson_oid_t oid;
    bson_oid_t *oid_t;
    bson_oid_from_string(&oid, oidStr.toLatin1());

    bson* bsrec = ejdbloadbson(m_col, &oid);

    //bson_print_raw(bsrec->data, 0);

    QJsonObject obj;

    bson_iterator *it = bson_iterator_create();
    bson_iterator_init(it, bsrec);


    bson_type bstype;
    while (bson_iterator_more(it)) {
        bstype = bson_iterator_next(it);
        const char* key = bson_iterator_key(it);
        qDebug() << key << ":" << bstype;

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
    bson_iterator_dispose(it);
    return obj;
}



bson QEJDBCollection::convert2Bson(QJsonObject obj)
{

    bson bsrec;
    bson_init(&bsrec);

    if (obj.contains("_id")) {

        bson_oid_t oid;
        bson_oid_from_string(&oid, obj.value("_id").toString().toLatin1());
        bson_append_oid(&bsrec, "_id", &oid);
    }

    QEJDBCollection::convert2Bson2(obj, bsrec);

    bson_finish(&bsrec);

    return bsrec;
}


void QEJDBCollection::convert2Bson2(QJsonObject obj, bson &bsrec)
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
                QEJDBCollection::convert2Bson2(jsonObj, bsrec);
                bson_append_finish_object(&bsrec);
                break;
        }

    }



}

