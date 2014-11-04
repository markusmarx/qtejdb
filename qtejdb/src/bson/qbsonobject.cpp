// license
#include "qbsonobject.h"
#include <QSharedData>
#include "bson.h"
#include "qbsonvalue.h"
#include "qbsonarray.h"
#include "qbsonoid.h"
#include "qbsonobject_p.h"
#include <QDateTime>
#include <QDebug>


/**
 * @brief QBsonObjectData::convert2BsonEntry convert the variant value in a bson repräsentation and
 * append to the given bson.
 *
 * @param bson pointer to a bson structur
 * @param attr attribute name to append.
 * @param value QBsonValue.
 */
void QBsonObjectData::convert2BsonEntry(bson *bson, const char* attr, QBsonValue &value)
{
    int i = 0;
    QList<QBsonValue> arr;
    QBsonValueHash obj;

    switch (value.type()) {
        case QBsonValue::Id:
            bson_oid_t oid;
            bson_oid_from_string(&oid, value.toId().toString().toLatin1());
            bson_append_oid(bson, attr, &oid);
            break;
        case QBsonValue::String:
            bson_append_string(bson, attr, value.toString().toLatin1()); break;
        case QBsonValue::Integer:
            bson_append_int(bson, attr, value.toInt()); break;
        case QBsonValue::Double:
            bson_append_double(bson, attr, value.toDouble()); break;
        case QBsonValue::Long:
            bson_append_long(bson, attr, value.toLong()); break;
        case QBsonValue::Bool:
            bson_append_bool(bson, attr, value.toBool()); break;
        case QBsonValue::DateTime:
            bson_append_date(bson, attr, value.toDateTime().toMSecsSinceEpoch());
        break;
        case QBsonValue::Object:
            bson_append_start_object(bson, attr);
            obj = value.toObject().values();
            for (QHash<QString, QBsonValue>::iterator it = obj.begin();
                 it != obj.end(); ++it) {
                convert2BsonEntry(bson, it.key().toLatin1(), it.value());
            }

            bson_append_finish_object(bson);
        break;
        case QBsonValue::Array:
            bson_append_start_array(bson, attr);
            arr = value.toArray().values();

            for (QList<QBsonValue>::iterator it = arr.begin();
                 it != arr.end(); ++it) {
                convert2BsonEntry(bson, QString::number(i).toLatin1(), *it);
            }

            bson_append_finish_array(bson);
        break;
        case QBsonValue::Binary: {
            QByteArray binary = value.toBinary();
            bson_append_binary(bson, attr, 2, binary.constData(), binary.size());
            break;
        }
        default: break;
    }
}

/**
 * @brief QBsonObjectData::convert2QBsonValue convert a bson type in a QBsonValue type.
 *
 * @param bt bson_type enum
 * @param it iterator that points to the value.
 *
 * @return QBsonValue()
 */
QBsonValue QBsonObjectData::convert2QBsonValue(bson_type bt, bson_iterator *it)
{

    switch (bt) {
        case BSON_BOOL:
            return QBsonValue(bson_iterator_bool(it));
        case BSON_LONG:
            return QBsonValue(bson_iterator_long(it));
        case BSON_INT:
            return QBsonValue(bson_iterator_int(it));
        case BSON_DOUBLE:
            return QBsonValue(bson_iterator_double(it));
        case BSON_STRING:
        case BSON_SYMBOL:
            return QBsonValue(QLatin1String(bson_iterator_string(it)));
        case BSON_OID: {
            char xoid[25];
            bson_oid_t* oid = bson_iterator_oid(it);
            bson_oid_to_string(oid, xoid);
            return QBsonValue(QBsonOid(xoid));
        }
        case BSON_ARRAY:
        case BSON_OBJECT:
            bson_iterator sit;
            BSON_ITERATOR_SUBITERATOR(it, &sit);
            if (bt == BSON_OBJECT) {
                QBsonObject subObj;
                convert2QBson2(subObj, &sit);
                return QBsonValue(subObj);
            } else {

                QBsonArray subArr;
                convert2QBson2(subArr, &sit);
                return QBsonValue(subArr);
            }
        case BSON_DATE:
            return QBsonValue(QDateTime::fromMSecsSinceEpoch(
                                  bson_iterator_long_raw(it)));
        case BSON_BINDATA: {

            //char type = bson_iterator_bin_type(it);
            return QBsonValue(QByteArray(bson_iterator_bin_data(it),
                                         bson_iterator_bin_len(it)));
        }
        default: return QBsonValue();
    }

}

/**
 * @brief QBsonObjectData::convert2QBson2 append values from a bson iterator to the array.
 *
 * @param obj QBsonArray
 * @param it iterator points to the array.
 */
void QBsonObjectData::convert2QBson2(QBsonArray &obj, bson_iterator *it)
{
    bson_type bt;
    while ((bt = bson_iterator_next(it)) != BSON_EOO)  {
        obj.append(QBsonValue(convert2QBsonValue(bt, it)));
    }

}

/**
 * @brief QBsonObjectData::convert2QJson2 append values from bson iterator to the object.
 *
 * @param obj QBsonObject
 * @param it iterator point to the object.
 */
void QBsonObjectData::convert2QBson2(QBsonObject &obj, bson_iterator *it)
{
    bson_type bt;

    while ((bt = bson_iterator_next(it)) != BSON_EOO) {
        QString key = BSON_ITERATOR_KEY(it);
        obj.insert(key, convert2QBsonValue(bt, it));
    }
}

/**
 * @brief QBsonObjectData::convert2QBson convert a bson to a QBsonObject. The converter uses the ejdb bson2json function.
 * So all datatypes should supported. Currently this is not fully tested.
 *
 * @param bson pointer to a bson structure.
 * @return converted QBsonObject.
 */
void QBsonObjectData::convert2QBson(bson *bson, QBsonObject& obj)
{

    bson_iterator it;

    BSON_ITERATOR_FROM_BUFFER(&it, bson->data);

    convert2QBson2(obj, &it);

}


/**
 * @brief QBsonObjectData::convert2Bson convert the given QBsonObject to a bson repräsentation.
 *
 * @param obj QBsonObject
 *
 * @return bson
 */
QByteArray QBsonObjectData::toBinary()
{

    bson bs = convert2Bson(*this);
    const char* binary = bson_data(&bs);
    int size = bson_size(&bs);

    QByteArray bin(binary, size);
    bson_destroy(&bs);
    return bin;
}

void QBsonObjectData::fromBinary(const QByteArray& binary, QBsonObject& obj)
{
    const char* data = binary.constData();
    bson bs;
    bson_init_with_data(&bs, data);
    bson_finish(&bs);
    convert2QBson(&bs, obj);

}

bson QBsonObjectData::convert2Bson(QBsonObjectData &obj, bson* bs)
{
    bson bsrec;

    if (bs == 0) {
        bson_init(&bsrec);
    } else {
        bsrec = *bs;
    }


    QBsonValueHash values = obj.values;
    if (values.contains("_id")) {

        bson_oid_t oid;
        bson_oid_from_string(&oid, values.value("_id").toString().toLatin1());
        bson_append_oid(&bsrec, "_id", &oid);
    }


    QBsonValueHash::Iterator it;

    for(it = values.begin(); it != values.end(); ++it) {

        QBsonObjectData::convert2Bson2(it.key().toLatin1(), it.value(), bsrec);
    }
    if (bs == 0) {
        bson_finish(&bsrec);
    }
    return bsrec;
}

/**
 * @brief convert2Bson2 recursive method to convert QBsonValue 2 bson record.
 *
 * @param attr  const *char attribute name.
 * @param value QBsonValue to convert.
 * @param bsrec pointer to the bson record.
 */
void QBsonObjectData::convert2Bson2(const char* attr, QBsonValue value, bson &bsrec)
{

    if (value.type() == QBsonValue::Object) {
        bson_append_start_object(&bsrec, attr);
        QBsonObject jsonObj = value.toObject();
        QStringList keys = jsonObj.names();

        foreach(QString key, keys) {
            QBsonValue v = jsonObj.value(key);
            QBsonObjectData::convert2Bson2(key.toLatin1(), v, bsrec);
        }

        bson_append_finish_object(&bsrec);
    } else if (value.type() ==QBsonValue::Array) {

       QList<QBsonValue> arr = value.toArray().values();

        QList<QBsonValue>::Iterator it;
        bson_append_start_array(&bsrec, attr);
        int i = 0;
        char kbuf[TCNUMBUFSIZ];
        for (it = arr.begin(); it != arr.end();++it) {
            bson_numstrn(kbuf, TCNUMBUFSIZ, i++);
            QBsonObjectData::convert2Bson2(kbuf, *it, bsrec);
        }

        bson_append_finish_array(&bsrec);
    } else {
        QBsonObjectData::convert2BsonEntry(&bsrec, attr, value);
    }

}



/**
 * @brief QBsonObject::QBsonObject construct a new Empty QBsonObject
 */
QBsonObject::QBsonObject() : data(new QBsonObjectData)
{

}

QBsonObject::QBsonObject(const QString &name, const QBsonValue &value): data(new QBsonObjectData)
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
    QBsonObjectData::fromBinary(bson, *this);
}

/**
 * @brief QBsonObject::QBsonObject construct a bson from the bsonrec.
 *
 * @param bsonRec bson record.
 */
QBsonObject::QBsonObject(void *bsonRec):
    data(new QBsonObjectData)
{
    QBsonObjectData::convert2QBson((bson*)bsonRec, *this);
}

/**
 * @brief QBsonObject::insert insert a name QBsonValue pair.
 *
 * @param name name of element.
 * @param value value of element.
 */
void QBsonObject::insert(const QString &name, const QBsonValue &value)
{
    data->values.insert(name, value);
}

/**
 * @brief QBsonObject::append append the value to the QBsonObject instance.
 *
 * @param name  value name
 * @param value value
 * @return Returns this bsonobject.
 */
QBsonObject QBsonObject::append(const QString &name, const QBsonValue &value)
{
    insert(name, value);
    return *this;
}

QBsonValue QBsonObject::value(const QString &name)
{
    return data->values.value(name);
}

bool QBsonObject::contains(const QString &name)
{
    return data->values.contains(name);
}

QStringList QBsonObject::names()
{
    return data->values.keys();
}

QBsonValueHash QBsonObject::values()
{
    return data->values;
}

QByteArray QBsonObject::toBinary()
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
 * @brief QBsonObject::QBsonObject create a shared instance.
 * @param rhs
 */
QBsonObject::QBsonObject(const QBsonObject &rhs): data(rhs.data)
{
    data->ref.ref();
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
    dbg << "test";
    return dbg.space();
}
