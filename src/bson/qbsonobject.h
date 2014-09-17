#ifndef QBSONOBJECT_H
#define QBSONOBJECT_H

#include <QSharedDataPointer>

class QBsonObjectData;

class QBsonObject
{
public:

    QBsonObject();
    QBsonObject(const QBsonObject &);
    QBsonObject &operator=(const QBsonObject &);
    ~QBsonObject();

    QBsonObject(QByteArray bson);
private:
    QSharedDataPointer<QBsonObjectData> data;
};

#endif // QBSONOBJECT_H
