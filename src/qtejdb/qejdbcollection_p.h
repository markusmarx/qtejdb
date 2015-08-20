#ifndef QEJDBCOLLECTION_P_H
#define QEJDBCOLLECTION_P_H

#include <QWeakPointer>
#include "qbson/qbsonobject.h"
#include "qejdbresult.h"

QT_FORWARD_DECLARE_CLASS(QEjdbDatabasePrivate)

class QEjdbCollectionPrivate
{
    friend class QEjdbDatabase;

public:

    bool exist();
    bool create();
    bool drop();
    QBsonObject load(QBsonOid oid);
    bool save(QBsonObject &object);
    bool remove(QBsonOid oid);
    QEjdbResult query(QBsonObject query, QBsonObject hints);

protected:

    QEjdbCollectionPrivate(QString collection, QSharedPointer<QEjdbDatabasePrivate> db);

    QWeakPointer<QEjdbDatabasePrivate> m_db;
    QString m_collection;
    bool isDbValid() const;

};


#endif // QEJDBCOLLECTION_P_H

