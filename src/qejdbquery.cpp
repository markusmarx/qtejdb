#include "qejdbquery.h"
#include <QSharedData>

class QEjdbQueryData : public QSharedData {
public:
};

QEjdbQuery::QEjdbQuery() : data(new QEjdbQueryData)
{
}

QEjdbQuery::QEjdbQuery(const QEjdbQuery &rhs) : data(rhs.data)
{
}

QEjdbQuery &QEjdbQuery::operator=(const QEjdbQuery &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

QEjdbQuery::~QEjdbQuery()
{
}
