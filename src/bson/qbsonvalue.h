#ifndef QBSONVALUE_H
#define QBSONVALUE_H

#include <QSharedDataPointer>

class QBsonValueData;

class QBsonValue
{
public:
    QBsonValue();
    QBsonValue(const QBsonValue &);
    QBsonValue &operator=(const QBsonValue &);
    ~QBsonValue();

private:
    QSharedDataPointer<QBsonValueData> data;
};

#endif // QBSONVALUE_H
