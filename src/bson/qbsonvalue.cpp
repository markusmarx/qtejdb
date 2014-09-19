#include "qbsonvalue.h"
#include <QSharedData>

class QBsonValueData : public QSharedData {
public:
};

QBsonValue::QBsonValue() : data(new QBsonValueData)
{
}

QBsonValue::QBsonValue(const QBsonValue &rhs) : data(rhs.data)
{
}

QBsonValue &QBsonValue::operator=(const QBsonValue &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

QBsonValue::~QBsonValue()
{
}
