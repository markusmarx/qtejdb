// license
#include "qbsonobject.h"
#include <QSharedData>
#include "bson.h"
#include "qbsonvalue.h"
#include "qbsonarray.h"
#include "qbsonoid.h"
#include "qbsonobject_p.h"
#include <QDateTime>
#include <QDataStream>
#include <QDebug>
#include "qbsontobsonrec.h"
#include "qbsonfrombsonrec.h"

void QBsonObjectData::insert(const QString &name, const QBsonValue &value)
{
    if (!values.contains(name)) {
        sortedNames.append(name);
    }
    values.insert(name, value);

}

/**
 * @brief QBsonObjectData::convert2Bson convert the given QBsonObject to a bson repräsentation.
 *
 * @param obj QBsonObject
 *
 * @return bson
 */
QByteArray QBsonObjectData::toBinary() const
{
    bson bs = convertQBsonToBsonRec(this, 0);
    const char* binary = bson_data(&bs);
    int size = bson_size(&bs);

    QByteArray bin(binary, size);
    bson_destroy(&bs);
    return bin;
}

void QBsonObjectData::fromBinary(const QByteArray &binary)
{
    const char* data = binary.constData();
    bson bs;
    bson_init_with_data(&bs, data);
    bson_finish(&bs);
    convertBsonObjectFromBsonRec(&bs, this);
}


/**
 * @brief QBsonObject::QBsonObject construct a new Empty QBsonObject
 */
QBsonObject::QBsonObject() : data(new QBsonObjectData)
{

}

QBsonObject::QBsonObject(const QString &name, const QBsonValue &value)
    : data(new QBsonObjectData)
{
    append(name, value);
}

/**
 * @brief QBsonObject::QBsonObject construct a QBsonObject from stream.
 * @param bson
 */
QBsonObject::QBsonObject(const QByteArray& bson):
    data(new QBsonObjectData)
{
    data->fromBinary(bson);
}

/**
 * @brief Insert the value to the QBsonObject instance.
 * Overwrites value if key exist.
 */
QBsonObject &QBsonObject::insert(const QString &name, const QBsonValue &value)
{
    data->insert(name, value);
    return *this;
}

/**
 * @brief QBsonObject::append append the value to the QBsonObject instance.
 * Overwrites value if key exist.
 *
 * @param name  value name
 * @param value value
 * @return Returns this bsonobject.
 */
QBsonObject &QBsonObject::append(const QString &name, const QBsonValue &value)
{
    insert(name, value);
    return *this;
}

/**
 * @brief Returns value stored at given property name or empty
 * value if property not exist.
 */
const QBsonValue QBsonObject::value(const QString &name) const
{
    if (data->values.contains(name)) {
        return data->values.value(name);
    }
    qWarning() << "bson value" << name << "not exist";
    return QBsonValue();
}

/**
 * @brief Returns true if the bson object has a property with
 * given name.
 */
bool QBsonObject::contains(const char *name) const
{
    return contains(QString(name));
}

/**
 * @brief Returns true if the bson object has a property with
 * given name.
 */
bool QBsonObject::contains(const QString &name) const
{
    return data->values.contains(name);
}

QStringList QBsonObject::names() const
{
    return data->sortedNames;
}

const QBsonValueHash QBsonObject::values() const
{
    return data->values;
}

QByteArray QBsonObject::toBinary() const
{

    return data->toBinary();

}

/**
 * @brief QBsonObject::remove remove name value pair.
 *
 * @param name value name
 *
 * @return true if success otherwise false.
 */
bool QBsonObject::remove(const QString &name)
{
    return data->values.remove(name) > 0;
}

/**
 * @brief QBsonObject::isEmpty test if the bson contains any value.
 *
 * @return true if the bson contains values otherwise false.
 */
bool QBsonObject::isEmpty() const
{
    return data->values.isEmpty();
}

/**
 * @brief QBsonObject::hasId returns true if a property _id exists.
 */
bool QBsonObject::hasOid() const
{
    return data->oid.isValid();
}

/**
 * @brief QBsonObject::oid returns the _id property or a empty QBsonOid if
 * not _id value exist.
 */
QBsonOid QBsonObject::oid() const
{
    return data->oid;
}

void QBsonObject::setOid(QBsonOid oid)
{
    data->oid = oid;
}


/**
 * @brief QBsonObject::operator ==c compare two BsonObjects
 * @param obj
 * @return
 */
bool QBsonObject::operator==(QBsonObject &obj)
{

    //QHash<QString, QBsonValue>::Iterator it = obj.values().iterator;
    return false;

}



/**
 * @brief QBsonObject::QBsonObject create a shared instance.
 * @param rhs
 */
QBsonObject::QBsonObject(const QBsonObject &rhs): data(rhs.data)
{
    if (!data) {
        data = new QBsonObjectData();
    } else {
        data->ref.ref();
    }
}

/**
 * @brief QBsonObject::operator = create a shared instance.
 * @param rhs
 * @return
 */
QBsonObject &QBsonObject::operator=(const QBsonObject &rhs)
{
    qAtomicAssign<QBsonObjectData>(data, rhs.data);
    return *this;
}

QBsonObject::~QBsonObject()
{
    if(!data->ref.deref())
        delete data;
}


QDebug operator<<(QDebug dbg, const QBsonObject &c)
{
    dbg << c.names();
    return dbg.space();
}


QDataStream &operator<<(QDataStream &d, const QBsonObject &object)
{

    d << object.toBinary();
    return d;
}


QDataStream &operator>>(QDataStream &d, QBsonObject &object)
{
    QByteArray ba;
    d >> ba;
    object = QBsonObject(ba);
    return d;
}


QDataStream &operator<<(QDataStream &d, const QList<QBsonObject> &objectList)
{
    d << objectList.size();

    QList<QBsonObject>::const_iterator it;
    for (it = objectList.begin(); it != objectList.end(); it++) {
        d << *it;
    }

    return d;
}


QDataStream &operator>>(QDataStream &d, QList<QBsonObject> &objectList)
{
    int size;
    d >> size;

    for (int i = 0; i < size; i++) {
        QBsonObject ob;
        d >> ob;
        objectList.append(ob);
    }

    return d;
}
