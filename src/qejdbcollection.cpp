#include "qejdbcollection.h"
#include <QDebug>
#include <QJsonDocument>
#include "globals_p.h"
#include "ejdb.h"

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


    bool save(QJsonObject &obj);
    bool remove(QJsonObject &obj);

    QJsonObject load(QString oidStr);

    bool removeCollection();

};



QEjdbCollection::QEjdbCollection(EJDB *db, EJCOLL *col, QString collectionName)
    :d(new QEjdbCollectionPrivate(this, db, col, collectionName))
{

}


bool QEjdbCollectionPrivate::save(QJsonObject &obj)
{
    bson_oid_t oid;

    // get bson
    /*bson bsrec = QEJDB::convert2Bson(obj);

    // save
    bool res = ejdbsavebson(m_col, &bsrec, &oid);
    char oidhex[25];
    bson_oid_to_string(&oid, oidhex);
    obj.insert("_id", QJsonValue(QString(oidhex)));
    bson_destroy(&bsrec);

    return res;*/
    return false;
}

bool QEjdbCollectionPrivate::remove(QJsonObject &obj)
{
    QString oidStr = obj.value("_id").toString();
    bson_oid_t oid;

    bson_oid_from_string(&oid, oidStr.toLatin1());
    return ejdbrmbson(m_col, &oid);
}

QJsonObject QEjdbCollectionPrivate::load(QString oidStr)
{

    bson_oid_t oid;
    bson_oid_from_string(&oid, oidStr.toLatin1());

    bson* bsrec = ejdbloadbson(m_col, &oid);

    /*QJsonObject obj = QEJDB::convert2QJson(bsrec);
    bson_del(bsrec);
*/
    QJsonObject obj;

    return obj;
}

bool QEjdbCollectionPrivate::removeCollection()
{
    if (!ejdbrmcoll(this->m_db, this->m_collectionName.toLatin1(), true)) {
        return false;
    }

    return true;
}

bool QEjdbCollection::save(QJsonObject &obj)
{
    return d->save(obj);
}

QJsonObject QEjdbCollection::load(QString oid)
{
    return d->load(oid);
}

bool QEjdbCollection::remove(QJsonObject &obj)
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

QEjdbCollection::~QEjdbCollection()
{
    qAtomicDetach<QEjdbCollectionPrivate>(d);
}

