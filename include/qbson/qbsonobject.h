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
    QBsonObject &insert(const QString &name, const QBsonValue &value);
    QBsonObject &append(const QString &name, const QBsonValue &value);
    const QBsonValue value(const QString &name) const;
    bool contains(const QString &name);
    QStringList names() const;
    QBsonValueHash values();
    QByteArray toBinary() const;
    bool remove(const QString& name);
    bool isEmpty() const;

    bool operator==(QBsonObject &obj);

private:
    friend class QObjectBsonData;
    friend class QEjdbCollectionPrivate;
    friend class QEjdbFileWorker;

    QBsonObjectData *data;

    QBsonObject(void* bsonRec, bool transferable = false);

    QBsonObjectData* constData() const {return data;}

};

Q_DECLARE_METATYPE(QBsonObject)
Q_DECLARE_METATYPE(QList<QBsonObject>)

QDebug operator<<(QDebug dbg, const QBsonObject &c);

QDataStream& operator<<(QDataStream& d, const QBsonObject& object);
QDataStream& operator>>(QDataStream& d, QBsonObject& object);

QDataStream& operator<<(QDataStream& d, const QList<QBsonObject>& objectList);
QDataStream& operator>>(QDataStream& d, QList<QBsonObject>& objectList);


#endif // QBSONOBJECT_H
