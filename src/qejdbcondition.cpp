#include "qejdbcondition.h"
#include <QSharedData>
#
class QEjdbConditionData : public QSharedData {
public:
    QEjdbConditionData(QString attribute, QEjdbCondition::ConditionType type, QVariant value)
        :m_attribute(attribute), m_type(type), m_value(value) {}

    QVariant m_value;
    QEjdbCondition::ConditionType m_type;
    QString m_attribute;

};

QEjdbCondition::QEjdbCondition(QString attribute, ConditionType type, QVariant value)
    : data(new QEjdbConditionData(attribute, type, value))
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

QString QEjdbCondition::attribute()
{
    return data->m_attribute;
}

QVariant QEjdbCondition::value()
{
    return data->m_value;
}

QEjdbCondition::ConditionType QEjdbCondition::type()
{
    return data->m_type;
}


