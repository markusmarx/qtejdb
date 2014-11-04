#include "qejdbquery.h"
#include "qejdbdatabase.h"
#include "qatomic.h"
#include "bson/qbsonobject.h"

class QEjdbQueryData {
public:

    QEjdbQueryData(const QEjdbCollection& db):collection(db)
    {
        ref = 1;
    }
    QString m_collectionName;

    QEjdbCollection collection;
    QAtomicInt ref;

};

QEjdbQuery::QEjdbQuery(const QEjdbCollection& db) : data(new QEjdbQueryData(db))
{

}

QEjdbQuery::QEjdbQuery(const QEjdbQuery &rhs) : data(rhs.data)
{
    data->ref.ref();
}

QEjdbQuery &QEjdbQuery::operator=(const QEjdbQuery &rhs)
{
    qAtomicAssign<QEjdbQueryData>(data, rhs.data);
    return *this;
}

QEjdbQuery::~QEjdbQuery()
{
    if (!data->ref.deref()) {
        delete data;
    }
}

/**
 * @brief QEjdbQuery::exec
 * @param collection
 * @param query
 * @return
 */
QList<QBsonObject> QEjdbQuery::exec(const QBsonObject& query)
{
    return data->collection.query(query);
}


QString QEjdbQuery::collectionName() const
{
    return data->m_collectionName;
}
