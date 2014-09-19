#ifndef QBSONARRAY_H
#define QBSONARRAY_H

#include <QSharedDataPointer>

class QBsonArrayData;

class QBsonArray
{
public:
    QBsonArray();
    QBsonArray(const QBsonArray &);
    QBsonArray &operator=(const QBsonArray &);
    ~QBsonArray();

private:
    QSharedDataPointer<QBsonArrayData> data;
};

#endif // QBSONARRAY_H
