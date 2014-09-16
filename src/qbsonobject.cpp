#include "qbsonobject.h"
#include <QSharedData>

class QBsonObjectData : public QSharedData {
public:
};

QBsonObject::QBsonObject() : data(new QBsonObjectData)
{
}

QBsonObject::QBsonObject(const QBsonObject &rhs) : data(rhs.data)
{
}

QBsonObject &QBsonObject::operator=(const QBsonObject &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

QBsonObject::~QBsonObject()
{
}
