#include "qejdbquery.h"
#include <QSharedData>
#include "qejdbcondition.h"

class QEjdbQueryData : public QSharedData {
public:
    QString m_collectionName;
    QList<QEjdbCondition> m_conditions;
};

QEjdbQuery::QEjdbQuery(QString collectionName) : data(new QEjdbQueryData)
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

/**
 * @brief QEjdbQuery::addCondition
 * @param condition
 * @return
 */
QEjdbQuery &QEjdbQuery::addCondition(QEjdbCondition &condition)
{
    data->m_conditions.append(condition);
    return *this;
}

/**
 * @brief QEjdbQuery::conditions
 * @return
 */
QList<QEjdbCondition>& QEjdbQuery::conditions()
{
    return data->m_conditions;
}

QString QEjdbQuery::collectionName() const
{
    return data->m_collectionName;
}
