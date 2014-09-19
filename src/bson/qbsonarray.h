#ifndef QBSONARRAY_H
#define QBSONARRAY_H

#include <QObject>
#include "qbsonvalue.h"

class QBsonArrayData;

class QBsonArray
{
public:
    QBsonArray();
    QBsonArray(const QBsonArray &);
    QBsonArray &operator=(const QBsonArray &);
    ~QBsonArray();

    void append(const QBsonValue &value);
    void insert(int i, const QBsonValue &value);
    QBsonValue value(int i);
    QList<QBsonValue> values() const;

private:
    QBsonArrayData *data;
};

Q_DECLARE_METATYPE(QBsonArray)
#endif // QBSONARRAY_H
