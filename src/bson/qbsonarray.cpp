#include "qbsonarray.h"
#include <QSharedData>
#include "qatomic.h"
#include <QLinkedList>
#include "qbsonvalue.h"

class QBsonArrayData {
public:
    QBsonArrayData() { ref = 1; }

    QAtomicInt ref;
    QList<QBsonValue> list;
};

QBsonArray::QBsonArray() : data(new QBsonArrayData)
{
}

QBsonArray::QBsonArray(const QBsonArray &rhs) : data(rhs.data)
{
    data->ref.ref();
}

QBsonArray &QBsonArray::operator=(const QBsonArray &rhs)
{
    qAtomicAssign(data, rhs.data);
    return *this;
}

QBsonArray::~QBsonArray()
{
    if (!data->ref.deref())
        delete data;
}

/**
 * @brief QBsonArray::append append a QBsonValue to the array.
 * @see QList::append()
 * @param value value to append
 */
void QBsonArray::append(const QBsonValue &value)
{
    data->list.append(value);
}

/**
 * @brief QBsonArray::insert insert a QBsonValue on given index.
 * @see QList::insert()
 * @param i insertion index.
 * @param value value to insert.
 */
void QBsonArray::insert(int i, const QBsonValue &value)
{
    data->list.insert(i, value);
}

/**
 * @brief QBsonArray::value return QBsonValue at given index.
 * @see QList::at(int)
 * @param i index
 * @return
 */
QBsonValue QBsonArray::value(int i)
{
    return data->list.at(i);
}

/**
 * @brief QBsonArray::values return all values.
 * @return QList<QBsonValue>()
 */
QList<QBsonValue> QBsonArray::values() const
{
    return data->list;
}
