#ifndef QEJDBCONDITION_H
#define QEJDBCONDITION_H

#include <QSharedDataPointer>

class QEjdbConditionData;

class QEjdbCondition
{
public:
    QEjdbCondition();
    QEjdbCondition(const QEjdbCondition &);
    QEjdbCondition &operator=(const QEjdbCondition &);
    ~QEjdbCondition();

private:
    QSharedDataPointer<QEjdbConditionData> data;
};

#endif // QEJDBCONDITION_H
