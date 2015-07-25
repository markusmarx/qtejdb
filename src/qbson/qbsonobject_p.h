#ifndef QBSONOBJECT_P_H
#define QBSONOBJECT_P_H
#include "qatomic.h"
#include "qbsonoid.h"
#include <QStringList>
#define TCNUMBUFSIZ 32

QT_FORWARD_DECLARE_CLASS(QBsonObject)

class Q_DECL_EXPORT QBsonObjectData  {

public:
    QBsonObjectData()
    {
        ref = 1;
    }

    QBsonValueHash values;
    QBsonOid oid;

    QStringList sortedNames;
    QByteArray toBinary() const;
    void fromBinary(const QByteArray& binary);
    void insert(const QString &name, const QBsonValue &value);

    QAtomicInt ref;
};


#endif // QBSONOBJECT_P_H
