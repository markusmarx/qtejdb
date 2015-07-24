#include "qbsonvalue.h"
#include <QSharedData>
#include "qbsonobject.h"
#include "qbsonarray.h"
#include "qbsonoid.h"
#include <QVariant>
#include "qatomic.h"
#include <QDebug>

class QBsonValueData {

public:

    QBsonValueData(QBsonValue::QBsonValueType t): type(t)
    {
        ref = 1;
    }

    QVariant v;

    const QBsonValue::QBsonValueType type;

    QAtomicInt ref;
};

/**
 * @brief QBsonValue construct a bsonvalue with type QBsonValueType::EMPTY
 */
QBsonValue::QBsonValue()
    : data(new QBsonValueData(QBsonValue::Empty))
{

}

/**
 * @brief QBsonValue::QBsonValue construct a bsonvalue with type
 * QBsonValue::INTEGER
 * @param value int value to store.
 */
QBsonValue::QBsonValue(int value)
    : data(new QBsonValueData(QBsonValue::Integer))
{
    data->v = value;
}

/**
 * @brief QBsonValue::QBsonValue construct a bsonvalue with type
 * QBsonValue::LONG
 * @param value long value to store.
 */
QBsonValue::QBsonValue(qlonglong value)
    : data(new QBsonValueData(QBsonValue::Long))
{
    data->v.setValue(value);
}

/**
 * @brief QBsonValue::QBsonValue construct a bsonvalue with type
 * QBsonValue::LONG
 * @param value long value to store.
 */
QBsonValue::QBsonValue(long value)
    : data(new QBsonValueData(QBsonValue::Long))
{
    data->v.setValue(value);
}

/**
 * @brief QBsonValue::QBsonValue construct a bsonvalue with type
 * QBsonValue::OBJECT
 * @param value object value to store.
 */
QBsonValue::QBsonValue(const QBsonObject &value)
    : data(new QBsonValueData(QBsonValue::Object))
{
    data->v.setValue(value);
}

/**
 * @brief QBsonValue::QBsonValue construct a bsonvalue with type
 * QBsonValue::STRING
 * @param value string value to store.
 */
QBsonValue::QBsonValue(const QString &value)
    : data(new QBsonValueData(QBsonValue::String))
{
    data->v = value;
}


/**
 * @brief QBsonValue construct a bsonvalue with type QBsonValue::BINARY
 * @param value binary value to store.
 */
QBsonValue::QBsonValue(const QByteArray &value):
    data(new QBsonValueData(QBsonValue::Binary))
{
    data->v = value;
}

/**
 * @brief QBsonValue construct a bsonvalue with type QBsonValue::Array
 * @param value array value to store.
 */
QBsonValue::QBsonValue(const QBsonArray &value):
    data(new QBsonValueData(QBsonValue::Array))
{
    data->v.setValue(value);
}

/**
 * @brief QBsonValue construct a bsonvalue with type QBsonValue::Bool
 * @param value bool value to store.
 */
QBsonValue::QBsonValue(bool value):
    data(new QBsonValueData(QBsonValue::Bool))
{
    data->v = value;
}

/**
 * @brief QBsonValue construct a bsonvalue with type QBsonValue::Id
 * @param value id value to store.
 */
QBsonValue::QBsonValue(const QBsonOid &value):
    data(new QBsonValueData(QBsonValue::Id))
{
    data->v.setValue(value);
}

/**
 * @brief QBsonValue construct a bsonvalue with type QBsonValue::STRING
 * @param value string value to store.
 */
QBsonValue::QBsonValue(const QLatin1String &value):
    data(new QBsonValueData(QBsonValue::String))
{
    data->v = value;
}

QBsonValue::QBsonValue(const char *value):
     data(new QBsonValueData(QBsonValue::String))
{
    data->v = QLatin1String(value);
}

/**
 * @brief QBsonValue construct a bsonvalue with type QBsonValue::DOUBLE
 * @param value double value to store.
 */
QBsonValue::QBsonValue(double value):
    data(new QBsonValueData(QBsonValue::Double))
{
    data->v = value;
}

/**
 * @brief QBsonValue construct a bsonvalue with type QBsonValue::DATETIME
 * @param value datetime value to store.
 */
QBsonValue::QBsonValue(const QDateTime &value):
    data(new QBsonValueData(QBsonValue::DateTime))
{
    data->v = value;
}

/**
 * @brief QBsonValue::QBsonValue copy constructor.
 * @param rhs
 */
QBsonValue::QBsonValue(const QBsonValue &rhs): data(rhs.data)
{
   data->ref.ref();
}

/**
 * @brief QBsonValue::operator = copy constructor.
 * @param rhs
 * @return
 */
QBsonValue &QBsonValue::operator=(const QBsonValue &rhs)
{
    qAtomicAssign<QBsonValueData>(data, rhs.data);
    return *this;
}

QBsonValue::~QBsonValue()
{
    // delete shared instance when not in used.
    if (data && !data->ref.deref()) {
        delete data;
        data = 0;
    }
}

QBsonValue::QBsonValueType QBsonValue::type() const
{
    return (data == 0)? QBsonValue::Empty : data->type;
}

/**
 * @brief QBsonValue::toString return the value as string. If QBsonValue is
 * invalid or could not convert to String then an empty QString is returned.
 * @see QVariant::toString()
 * @return QString
 */
QString QBsonValue::toString() const
{
    if (isValid()) {
        switch(data->type) {
            case QBsonValue::Id:
                return toId().toString();
            default: {
                if (data->v.canConvert(QVariant::String)) {
                    return data->v.toString();
                }
            }
        }
    }
    return QString();
}

/**
 * @brief QBsonValue::toDouble return the value as double. If QBsonValue is
 * invlalid or could not convert to double a 0 is returned.
 * @see QVariant::toDouble()
 * @return double
 */
double QBsonValue::toDouble() const
{
    if (data->v.canConvert(QVariant::Double)) {
        return data->v.toDouble();
    }
    qWarning() << "QBsonValue cannot convert to double";
    return 0;
}

/**
 * @brief QBsonValue::toInt return the value as int. If QBsonValue is invalid
 * or could not convert to int a 0 is returned.
 * @see QVariant::toInt()
 * @return int
 */
int QBsonValue::toInt() const
{
    if (data->v.canConvert(QVariant::Int)) {
        return data->v.toInt();
    }
    qWarning() << "QBsonValue cannot convert to int";
    return 0;
}

/**
 * @brief QBsonValue::toLong return the value as long. If QBsonValue is invalid
 * or could not convert to long 0 is returned.
 * @see QVariant::toLongLong()
 * @return long
 */
long QBsonValue::toLong() const
{
    if (data->v.canConvert(QVariant::LongLong)) {
        return data->v.toLongLong();
    }
    qWarning() << "QBsonValue cannot convert to long";
    return 0;
}

/**
 * @brief QBsonValue::toBinary() return the value as QByteArray. If QBsonValue
 * is invalid or could not convert to QByteArray returns an empty binary.
 * @see QVariant::toByteArray()
 * @return QByteArray
 */
QByteArray QBsonValue::toBinary() const
{
    if (isValid(QBsonValue::Binary)) {
        return data->v.toByteArray();
    }
    qWarning() << "QBsonValue cannot convert to QByteArray";
    return QByteArray();
}

/**
 * @brief QBsonValue::toBsonObject() return the value as QBsonObject. If
 * QBsonValue is invalid or could not convert to QBsonObject returns an
 * empty QBsonObject.
 * @see QVariant::value()
 * @return QBsonObject
 */
QBsonObject QBsonValue::toObject() const
{
    if (isValid(QBsonValue::Object)) {
        return data->v.value<QBsonObject>();
    }
    qWarning() << "QBsonValue cannot convert to QBsonObject";
    return QBsonObject();
}

/**
 * @brief QBsonValue::toDateTime() return the value as QDateTime. If QBsonValue
 * is invalid of could not convert to QDateTime returns a default QDateTime().
 *
 * @see QVariant::toDateTime()
 * @return QDateTime
 */
QDateTime QBsonValue::toDateTime() const
{
    if (isValid(QBsonValue::DateTime)) {
        return data->v.toDateTime();
    }
    qWarning() << "QBsonValue cannot convert to QDateTime";
    return QDateTime();
}

/**
 * @brief QBsonValue::toArray() return the value as QBsonArray. If QBsonValue
 * is invalid of could not convert to QBsonArray returns a empty QBsonArray.
 * @see QVariant::value()
 * @return QBsonArray
 */
QBsonArray QBsonValue::toArray() const
{
    if (isValid(QBsonValue::Array)) {
        return data->v.value<QBsonArray>();
    }
    qWarning() << "QBsonValue cannot convert to array";
    return QBsonArray();
}

/**
 * @brief QBsonValue::toBool return the value as bool. If QBsonValue is invalid
 * or could not convert to boolean returns a false boolean.
 *
 * @see QVariant::toBool()
 * @return bool
 */
bool QBsonValue::toBool() const
{
    if (isValid(QBsonValue::Bool)) {
        return data->v.toBool();
    }
    qWarning() << "QBsonValue cannot convert to bool";
    return false;
}

/**
 * @brief QBsonValue::toId return the value as QBsonOid. If BsonValue is invalid
 * or could not convert to a QBsonOid return a invalid QBsonOid.
 * @see QVariant::value()
 * @return QBsonOid
 */
QBsonOid QBsonValue::toId() const
{
    if (isValid(QBsonValue::Id)) {
        return data->v.value<QBsonOid>();
    }
    qWarning() << "QBsonValue cannot convert to QBsonOid";
    return QBsonOid();
}


/**
 * @brief QBsonValue::isObject returns true if type is QBsonValue::Object
 *
 * @return true if type is QBsonValue::Object otherwise false.
 */
bool QBsonValue::isObject() const
{
    return isValid(QBsonValue::Object);
}

/**
 * @brief QBsonValue::isArray returns true if type id QBsonValue::Array
 * @return true if type id QBsonValue::Array otherwise false.
 */
bool QBsonValue::isArray() const
{
    return isValid(QBsonValue::Array);
}

/**
 * @brief QBsonValue::isValid Returns true when the value is valid else false.
 * @return true when the value is valid else false.
 */
bool QBsonValue::isValid() const
{
    return data != 0 && data->v.isValid();
}

/**
 * @brief QBsonValue::toVariant convert the value to a qvariant. if the value
 * could not convert a invalid qvariant is returned.
 *
 * @return QVariant()
 */
QVariant QBsonValue::toVariant() const
{
    return data->v;
}

/**
 * @brief QBsonValue::fromVariant converts a variant value to bsonvalue or
 * a default constructed QBsonValue if vaiant could not converted.
 * @param variant variant to convert
 * @return
 */
QBsonValue QBsonValue::fromVariant(QVariant variant)
{
    switch (variant.type()) {
    case QVariant::Bool:
        return QBsonValue(variant.toBool());
    case QVariant::ByteArray:
        return QBsonValue(variant.toByteArray());
    case QVariant::String:
        return QBsonValue(variant.toString());
    case QVariant::LongLong:
        return QBsonValue(variant.toLongLong());
    case QVariant::Int:
        return QBsonValue(variant.toInt());
    case QVariant::Double:
        return QBsonValue(variant.toDouble());
    case QVariant::DateTime:
        return QBsonValue(variant.toDateTime());
    }
    if (variant.canConvert<QBsonObject>()) {
        return QBsonValue(variant.value<QBsonObject>());
    }
    if (variant.canConvert<QBsonOid>()) {
        return QBsonValue(variant.value<QBsonOid>());
    }
    if (variant.canConvert<QBsonArray>()) {
        return QBsonValue(variant.value<QBsonArray>());
    }

    return QBsonValue();
}

/**
 * @brief QBsonValue::isValid Returns true if the value is valid and given type.
 *
 * @param type value type
 *
 * @return true if the value is valid and given type.
 */
bool QBsonValue::isValid(QBsonValue::QBsonValueType type) const
{
    return isValid() && data->type == type;
}

bool QBsonValue::operator ==(const QBsonValue &value) const
{
    return data->v == value.data->v;
}

bool QBsonValue::operator ==(const QVariant value) const
{
    return data->v == value;
}

bool operator ==(const QVariant &variant, const QBsonValue &value)
{
    return variant == value.toVariant();
}
