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

const QList<QBsonObject> QEjdbResult::values() const
{
    QList<QBsonObject> resultBsonList;
    data->currentIt = data->values.begin();

    for (; data->currentIt != data->values.end();) {
        resultBsonList.append(data->nextBsonValue());
    }

    return resultBsonList;
}

QBsonObject QEjdbResult::first()
{
    return data->firstBsonValue();
}

int QEjdbResult::count()
{
    return data->values.count();
}

QEjdbResultData *QEjdbResult::d()
{
    return data;
}

QLinkedList<QVariant> QEjdbResult::valuesPrivate() const
{
    return data->values;
}

QDataStream &operator<<(QDataStream &d, const QEjdbResult &object)
{
    d << object.valuesPrivate();
    //qDebug() << "<<" <<object.valuesPrivate().size();
    return d;
}


QDataStream &operator>>(QDataStream &d, QEjdbResult &object)
{
    QLinkedList<QVariant> values;
    d >> values;
    object.d()->values = values;
    //qDebug() << ">>" << values.size();
    return d;
}


QBsonObject QEjdbResultData::nextBsonValue()
{
    QVariant variant = *currentIt;

    if (variant.type() == QVariant::ByteArray) {
        QBsonObject obj = QBsonObject(variant.toByteArray());

        values.insert(currentIt, QVariant::fromValue(obj));
        currentIt = values.erase(currentIt);
        return obj;
    }

    QBsonObject obj = (*currentIt).value<QBsonObject>();
    currentIt++;
    return obj;

}

QBsonObject QEjdbResultData::firstBsonValue()
{
    currentIt = values.begin();
    return nextBsonValue();
}
