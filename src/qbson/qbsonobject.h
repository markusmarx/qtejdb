#ifndef QBSONOBJECT_H
#define QBSONOBJECT_H
#include <QVariant>
#include <QSharedDataPointer>
#include "qbsonvalue.h"
#include "qbsonoid.h"
#include <QHash>
#include <stdarg.h>
#include <QStringList>

typedef QHash<QString, QBsonValue> QBsonValueHash;
class QBsonObjectData;
class QBsonOid;

class Q_DECL_EXPORT QBsonObject
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
    bool contains(const QString &name) const;
    bool contains(const char *name) const;
    QStringList names() const;
    const QBsonValueHash values() const;
    QByteArray toBinary() const;
    bool remove(const QString& name);
    bool isEmpty() const;
    bool hasOid() const;
    QBsonOid oid() const;
    void setOid(QBsonOid oid);

    bool operator==(const QBsonObject &obj) const;
protected:

    QBsonObjectData* constData() const {return data;}

    QBsonObjectData *data;

    friend class QBsonObjectData;
    friend class QEjdbCollectionPrivate;
    friend class QEjdbFileWorker;

};

Q_DECLARE_METATYPE(QBsonObject)
Q_DECLARE_METATYPE(QList<QBsonObject>)

QDebug Q_DECL_EXPORT operator<<(QDebug dbg, const QBsonObject &c);

Q_DECL_EXPORT QDataStream& operator<<(QDataStream& d, const QBsonObject& object);
Q_DECL_EXPORT QDataStream& operator>>(QDataStream& d, QBsonObject& object);

Q_DECL_EXPORT QDataStream& operator<<(QDataStream& d, const QList<QBsonObject>& objectList);
Q_DECL_EXPORT QDataStream& operator>>(QDataStream& d, QList<QBsonObject>& objectList);


#endif // QBSONOBJECT_H
