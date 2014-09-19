#ifndef QBSONOID_H
#define QBSONOID_H

#include <QSharedDataPointer>

class QBsonOidData;

class QBsonOid
{
public:
    QBsonOid();
    QBsonOid(const QBsonOid &);
    QBsonOid &operator=(const QBsonOid &);
    ~QBsonOid();

private:
    QSharedDataPointer<QBsonOidData> data;
};

#endif // QBSONOID_H
