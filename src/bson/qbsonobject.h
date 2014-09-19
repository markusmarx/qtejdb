#ifndef QBSONOBJECT_H
#define QBSONOBJECT_H
#include <QVariant>
#include <QSharedDataPointer>
#include "qbsonvalue.h"
#include "qbsonoid.h"
#include <QHash>


typedef QHash<QString, QBsonValue> QBsonValueHash;
class QBsonObjectData;
class QBsonOid;
class QBsonObject
{
public:

    QBsonObject();
    QBsonObject(const QBsonObject &);
    QBsonObject &operator=(const QBsonObject &);
    ~QBsonObject();

    QBsonObject(const QByteArray& bson);
    void insert(const QString &name, const QBsonValue &value);
    QBsonValue value(const QString &name);
    bool contains(const QString &name);
    QStringList names();
    QBsonValueHash values();
    QByteArray toBinary();

private:
    QBsonObjectData *data;
};

Q_DECLARE_METATYPE(QBsonObject)

#endif // QBSONOBJECT_H
