#ifndef QEJDBQUERY_H
#define QEJDBQUERY_H

#include <QSharedDataPointer>

class QEjdbQueryData;

class QEjdbQuery
{
public:
    QEjdbQuery();
    QEjdbQuery(const QEjdbQuery &);
    QEjdbQuery &operator=(const QEjdbQuery &);
    ~QEjdbQuery();

private:
    QSharedDataPointer<QEjdbQueryData> data;
};

#endif // QEJDBQUERY_H
