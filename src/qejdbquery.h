#ifndef QEJDBQUERY_H
#define QEJDBQUERY_H

#include <QSharedDataPointer>
#include "qejdbcondition.h"

class QEjdbQueryData;

class QEjdbQuery
{
public:
    QEjdbQuery(QString collectionName);
    QEjdbQuery(const QEjdbQuery &);
    QEjdbQuery &operator=(const QEjdbQuery &);
    ~QEjdbQuery();

    QEjdbQuery& addCondition(QEjdbCondition &condition);
    QList<QEjdbCondition> &conditions();
    QString collectionName() const;
private:
    QSharedDataPointer<QEjdbQueryData> data;
};

#endif // QEJDBQUERY_H
