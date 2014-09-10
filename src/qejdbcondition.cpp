#include "qejdbcondition.h"
#include <QSharedData>

class QEjdbConditionData : public QSharedData {
public:
};

QEjdbCondition::QEjdbCondition() : data(new QEjdbConditionData)
{
}

QEjdbCondition::QEjdbCondition(const QEjdbCondition &rhs) : data(rhs.data)
{
}

QEjdbCondition &QEjdbCondition::operator=(const QEjdbCondition &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

QEjdbCondition::~QEjdbCondition()
{
}
