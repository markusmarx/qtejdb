#ifndef QEJDBRESULT_H
#define QEJDBRESULT_H
#include <QtGlobal>
#include "qbson/qbsonobject.h"

QT_FORWARD_DECLARE_CLASS(QEjdbFileWorker)
QT_FORWARD_DECLARE_CLASS(QEjdbRpcWorker)
QT_FORWARD_DECLARE_CLASS(QEjdbResultData)

class Q_DECL_EXPORT QEjdbResult
{
    friend class QEjdbFileWorker;
    friend class QEjdbRpcWorker;
    friend QDataStream& operator>>(QDataStream& d, QEjdbResult& object);
    friend QDataStream& operator<<(QDataStream& d, const QEjdbResult& object);

public:
    explicit QEjdbResult();

    QEjdbResult(const QEjdbResult &rhs);
    QEjdbResult &operator =(const QEjdbResult &rhs);
    ~QEjdbResult();

    bool isEmpty();

    const QList<QBsonObject> values() const;

    QBsonObject first();
    QBsonObject next();
    bool hasNext();
    int count();

protected:
    QEjdbResultData *d();
    QLinkedList<QVariant> valuesPrivate() const;
private:
    QEjdbResultData *data;


};

Q_DECLARE_METATYPE(QEjdbResult)

Q_DECL_EXPORT QDataStream& operator<<(QDataStream& d, const QEjdbResult& object);
Q_DECL_EXPORT QDataStream& operator>>(QDataStream& d, QEjdbResult& object);


#endif // QEJDBRESULT_H
