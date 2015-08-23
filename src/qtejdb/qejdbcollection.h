#ifndef QEJDBCOLLECTION_H
#define QEJDBCOLLECTION_H
#include <QObject>
#include <QWeakPointer>
#include "qbson/qbsonobject.h"
#include "qejdbresult.h"

QT_FORWARD_DECLARE_CLASS(QEjdbDatabase)
QT_FORWARD_DECLARE_CLASS(QEjdbCollectionPrivate)

class Q_DECL_EXPORT QEjdbCollection
{
    friend class QEjdbDatabase;
public:
    QEjdbCollection(const QEjdbCollection &other);
    QEjdbCollection(){}
    bool exist();
    bool create();
    bool drop();
    QBsonObject load(QBsonOid oid);
    bool save(QBsonObject &object);
    bool remove(QBsonOid oid);
    QEjdbResult query(QBsonObject query, QBsonObject hints = QBsonObject());
protected:

    QEjdbCollection(QSharedPointer<QEjdbCollectionPrivate> od):d(od) {
    }


    QEjdbCollection &operator=(const QEjdbCollection &other)
    {
        d = other.d;
        return *this;
    }

    QSharedPointer<QEjdbCollectionPrivate> d;

};
#endif
