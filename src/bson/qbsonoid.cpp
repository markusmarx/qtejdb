#include "qbsonoid.h"
#include <QSharedData>

class QBsonOidData : public QSharedData {
public:
};

QBsonOid::QBsonOid() : data(new QBsonOidData)
{
}

QBsonOid::QBsonOid(const QBsonOid &rhs) : data(rhs.data)
{
}

QBsonOid &QBsonOid::operator=(const QBsonOid &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

QBsonOid::~QBsonOid()
{
}
