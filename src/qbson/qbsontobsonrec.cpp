
#include "qbsontobsonrec.h"
#include "qbsonarray.h"
#include "qbsonobject_p.h"

/**
 * @brief QBsonObjectData::convert2BsonEntry convert the variant value in a bson repräsentation and
 * append to the given bson.
 *
 * @param bson pointer to a bson structur
 * @param attr attribute name to append.
 * @param value QBsonValue.
 */
void appendQBsonValueToBsonRec(bson *bson, const char* attr, const QBsonValue &value)
{
    int i = 0;
    QList<QBsonValue> arrValues;
    QBsonValueHash objValues;
    QBsonObject subObject;

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
            subObject = value.toObject();
            objValues = value.toObject().values();
            if (subObject.hasOid()) {
                appendQBsonValueToBsonRec(bson, "_id", subObject.oid());
            }
            for(QBsonValueHash::Iterator it = objValues.begin(); it != objValues.end(); ++it) {
                appendQBsonValueToBsonRec(bson, it.key().toLatin1(), *it);
            }
            bson_append_finish_object(bson);
        break;
        case QBsonValue::Array:
            bson_append_start_array(bson, attr);
            arrValues = value.toArray().values();

            for (QList<QBsonValue>::iterator it = arrValues.begin();
                 it != arrValues.end(); ++it) {
                appendQBsonValueToBsonRec(bson, QString::number(i).toLatin1(), *it);
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

bson convertQBsonToBsonRec(const QBsonObject &obj, bson* bs)
{
    return convertQBsonToBsonRec(obj.values(), obj.oid(), bs);
}

bson convertQBsonToBsonRec(const QBsonObjectData *obj, bson* bs)
{
    return convertQBsonToBsonRec(obj->values, obj->oid, bs);
}

bson convertQBsonToBsonRec(QBsonValueHash values, QBsonOid oid, bson* bs)
{
    bson bsrec;

    if (bs == 0) {
        bson_init(&bsrec);
    } else {
        bsrec = *bs;
    }

    QBsonValueHash::Iterator it;

    if (oid.isValid()) {
        appendQBsonValueToBsonRec(&bsrec, "_id", oid);
    }

    for(it = values.begin(); it != values.end(); ++it) {
        appendQBsonValueToBsonRec(&bsrec, it.key().toLatin1(), *it);
    }

    if (bs == 0) {
        bson_finish(&bsrec);
    }
    return bsrec;
}

