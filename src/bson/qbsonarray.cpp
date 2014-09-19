#include "qbsonarray.h"
#include <QSharedData>

class QBsonArrayData : public QSharedData {
public:
};

QBsonArray::QBsonArray() : data(new QBsonArrayData)
{
}

QBsonArray::QBsonArray(const QBsonArray &rhs) : data(rhs.data)
{
}

QBsonArray &QBsonArray::operator=(const QBsonArray &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

QBsonArray::~QBsonArray()
{
}
