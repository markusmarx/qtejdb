#ifndef QBSONOBJECT_H
#define QBSONOBJECT_H
#include <QVariant>
#include <QSharedDataPointer>
#include "qbsonvalue.h"
#include "qbsonoid.h"
#include <QHash>
#include <stdarg.h>


typedef QHash<QString, QBsonValue> QBsonValueHash;
class QBsonObjectData;
class QBsonOid;

class QBsonObject
{
public:

    QBsonObject();
    QBsonObject(const QString &name, const QBsonValue &value);
    QBsonObject(const QBsonObject &);
    QBsonObject &operator=(const QBsonObject &);
    ~QBsonObject();

    QBsonObject(const QByteArray& bson);
    void insert(const QString &name, const QBsonValue &value);
    QBsonObject append(const QString &name, const QBsonValue &value);
    QBsonValue value(const QString &name);
    bool contains(const QString &name);
    QStringList names();
    QBsonValueHash values();
    QByteArray toBinary();
    bool remove(const QString& name);

private:
    friend class QObjectBsonData;
    friend class QEjdbCollectionPrivate;
    QBsonObjectData *data;

    QBsonObject(void* bsonRec);

    QBsonObjectData* constData() const {return data;}

};

Q_DECLARE_METATYPE(QBsonObject)

QDebug operator<<(QDebug dbg, const QBsonObject &c);

#endif // QBSONOBJECT_H
