#ifndef QEJDBCONDITION_H
#define QEJDBCONDITION_H

#include <QSharedDataPointer>
#include <QVariant>
class QEjdbConditionData;

class QEjdbCondition
{

public:

    enum ConditionType {
        BEGIN,
        IN,
        EQUALS,
        GT, GTE,
        LT, LTE
    };


    QEjdbCondition(QString attribute, ConditionType type, QVariant value);
    QEjdbCondition(const QEjdbCondition &);
    QEjdbCondition &operator=(const QEjdbCondition &);
    ~QEjdbCondition();

    QString attribute() const;
    ConditionType type() const;
    QVariant value() const;

private:
    QSharedDataPointer<QEjdbConditionData> data;
};

#endif // QEJDBCONDITION_H
