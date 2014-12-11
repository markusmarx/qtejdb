#ifndef QEJDBRESULT_H
#define QEJDBRESULT_H
#include <QtGlobal>
#include "qbsonobject.h"

QT_FORWARD_DECLARE_CLASS(QEjdbFileWorker)
QT_FORWARD_DECLARE_CLASS(QEjdbRpcWorker)
QT_FORWARD_DECLARE_CLASS(QEjdbResultData)

class QEjdbResult
{
    friend class QEjdbFileWorker;
    friend class QEjdbRpcWorker;
    friend QDataStream& operator>>(QDataStream& d, QEjdbResult& object);

public:
    explicit QEjdbResult();

    QEjdbResult(const QEjdbResult &rhs);
    QEjdbResult &operator =(const QEjdbResult &rhs);
    ~QEjdbResult();

    bool isEmpty();

    QList<QBsonObject> values() const;
    QBsonObject first();
    int count();

protected:
    QEjdbResultData *d();
    QLinkedList<QVariant> valuesPrivate() const;
private:
    QEjdbResultData *data;


};

Q_DECLARE_METATYPE(QEjdbResult)

QDataStream& operator<<(QDataStream& d, const QEjdbResult& object);
QDataStream& operator>>(QDataStream& d, QEjdbResult& object);


#endif // QEJDBRESULT_H
