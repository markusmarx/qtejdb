#ifndef QBSONARRAY_H
#define QBSONARRAY_H

#include <QObject>
#include "qbsonvalue.h"

class QBsonArrayData;

class Q_DECL_EXPORT QBsonArray
{
public:
    QBsonArray();
    QBsonArray(const QBsonArray &);
    QBsonArray &operator=(const QBsonArray &);
    ~QBsonArray();

    QBsonArray& append(const QBsonValue &value);
    QBsonArray& insert(int i, const QBsonValue &value);
    QBsonArray& remove(int i);
    QBsonValue take(int i);
    QBsonValue value(int i);
    QList<QBsonValue> values() const;
    int size() const;
    QList<QBsonValue> collectProperties(const QString &property) const;
    void replace(int index, const QBsonValue &value);

private:
    QBsonArrayData *data;

    inline bool inRange(int i);
};

Q_DECLARE_METATYPE(QBsonArray)
#endif // QBSONARRAY_H
