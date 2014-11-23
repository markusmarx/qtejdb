#include "qejdbresult.h"
#include "qejdbresult_p.h"
#include <QDebug>

QEjdbResult::QEjdbResult():data(new QEjdbResultData)
{
}

/**
 * @brief QBsonObject::QBsonObject create a shared instance.
 * @param rhs
 */
QEjdbResult::QEjdbResult(const QEjdbResult &rhs): data(rhs.data)
{

    if (!data) {
        data = new QEjdbResultData();
    } else {
        data->ref.ref();
    }
}

/**
 * @brief QEjdbResult::operator = create a shared instance.
 * @param rhs
 * @return
 */
QEjdbResult &QEjdbResult::operator=(const QEjdbResult &rhs)
{
    qAtomicAssign<QEjdbResultData>(data, rhs.data);
    return *this;
}

QEjdbResult::~QEjdbResult()
{
    if(!data->ref.deref())
        delete data;
}

QList<QBsonObject> QEjdbResult::values() const
{
    return data->values;
}

QBsonObject QEjdbResult::first()
{
    return data->values.first();
}

int QEjdbResult::count()
{
    return data->values.count();
}

QEjdbResultData *QEjdbResult::d()
{
    return data;
}

QDataStream &operator<<(QDataStream &d, const QEjdbResult &object)
{
    d << object.values();
    return d;
}


QDataStream &operator>>(QDataStream &d, QEjdbResult &object)
{

    QList<QBsonObject> values;
    d >> values;
    object.d()->values = values;
    return d;
}
