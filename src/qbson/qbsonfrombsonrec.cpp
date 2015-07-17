#include "qbsonfrombsonrec.h"
#include "qbsonarray.h"
#include <QDebug>


/**
 * @brief QBsonObjectData::convert2QBsonValue convert a bson type in a QBsonValue type.
 *
 * @param bt bson_type enum
 * @param it iterator that points to the value.
 *
 * @return QBsonValue()
 */
QBsonValue convertQBsonValueFromBsonRec(bson_type bt, bson_iterator *it)
{

    switch (bt) {
        case BSON_BOOL:
            // iterator return 1 or 0
            return QBsonValue(bson_iterator_bool(it) ? true: false);
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
            return QBsonOid(xoid);
        }
        case BSON_ARRAY:
        case BSON_OBJECT:
            bson_iterator sit;
            BSON_ITERATOR_SUBITERATOR(it, &sit);
            if (bt == BSON_OBJECT) {
                QBsonObject subObj;
                convertBsonValueFromBsonRec(subObj, &sit);
                return QBsonValue(subObj);
            } else {
                QBsonArray subArr;
                convertBsonValueFromBsonRec(subArr, &sit);
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
void convertBsonValueFromBsonRec(QBsonArray &obj, bson_iterator *it)
{
    bson_type bt;
    while ((bt = bson_iterator_next(it)) != BSON_EOO)  {
        obj.append(QBsonValue(convertQBsonValueFromBsonRec(bt, it)));
    }

}

/**
 * @brief QBsonObjectData::convert2QJson2 append values from bson iterator to the object.
 *
 * @param obj QBsonObject
 * @param it iterator point to the object.
 */
void convertBsonValueFromBsonRec(QBsonObjectData *objData, bson_iterator *it)
{
    bson_type bt;

    while ((bt = bson_iterator_next(it)) != BSON_EOO) {
        QString key = BSON_ITERATOR_KEY(it);
        if (key == "_id") {
            objData->oid = convertQBsonValueFromBsonRec(bt, it).toId();
            continue;
        }
        objData->insert(key, convertQBsonValueFromBsonRec(bt, it));
    }
}

/**
 * @brief QBsonObjectData::convert2QJson2 append values from bson iterator to the object.
 *
 * @param obj QBsonObject
 * @param it iterator point to the object.
 */
void convertBsonValueFromBsonRec(QBsonObject &obj, bson_iterator *it)
{
    bson_type bt;

    while ((bt = bson_iterator_next(it)) != BSON_EOO) {
        QString key = BSON_ITERATOR_KEY(it);
        if (key == "_id") {
            obj.setOid(convertQBsonValueFromBsonRec(bt, it).toId());
            continue;
        }
        obj.insert(key, convertQBsonValueFromBsonRec(bt, it));
    }
}

/**
 * @brief QBsonObjectData::convert2QBson convert a bson to a QBsonObject. The converter uses the ejdb bson2json function.
 * So all datatypes should supported. Currently this is not fully tested.
 *
 * @param bson pointer to a bson structure.
 * @return converted QBsonObject.
 */
void convertBsonObjectFromBsonRec(bson *bson, QBsonObjectData *obj)
{
    if (bson == 0) {
        qWarning() << "bson structure is empty.";
        return;
    }
    bson_iterator it;
    BSON_ITERATOR_FROM_BUFFER(&it, bson->data);

    convertBsonValueFromBsonRec(obj, &it);
}

/**
 * @brief QBsonObjectData::convert2QBson convert a bson to a QBsonObject. The converter uses the ejdb bson2json function.
 * So all datatypes should supported. Currently this is not fully tested.
 *
 * @param bson pointer to a bson structure.
 * @return converted QBsonObject.
 */
void convertBsonObjectFromBsonRec(bson *bson, QBsonObject &obj)
{
    if (bson == 0) {
        qWarning() << "bson structure is empty.";
        return;
    }
    bson_iterator it;
    BSON_ITERATOR_FROM_BUFFER(&it, bson->data);

    convertBsonValueFromBsonRec(obj, &it);
}
