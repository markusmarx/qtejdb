#include "qejdbquery.h"
#include "qejdbdatabase.h"
#include "qatomic.h"
#include "qbsonobject.h"

class QEjdbQueryData {
public:

    QEjdbQueryData(const QString &collectionName, const QEjdbDatabase& db):m_collectionName(collectionName), m_db(db)
    {
        ref = 1;
    }
    QString m_collectionName;

    QEjdbDatabase m_db;
    QAtomicInt ref;

};

QEjdbQuery::QEjdbQuery(const QString &collectionName, const QEjdbDatabase& db) : data(new QEjdbQueryData(collectionName, db))
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
QEjdbResult QEjdbQuery::exec(const QBsonObject& query)
{
    return data->m_db.query(data->m_collectionName, query);
}


QString QEjdbQuery::collectionName() const
{
    return data->m_collectionName;
}
