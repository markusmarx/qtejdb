#include "qbsonconverter.h"
#include <QJSValueIterator>

QBsonConverter::QBsonConverter()
{

}

QBsonConverter::~QBsonConverter()
{

}

/**
 * @brief QBsonConverter::convert converts a bsonObject to jsValue. The
 * converted QJSValue is a object.
 *
 * @param bsonObject
 *
 * @return QJSValue
 */
QJSValue QBsonConverter::convert(const QBsonObject &bsonObject)
{
    return mapObject(bsonObject);
}

/**
 * @brief QBsonConverter::convert converts a jsValue to bson object. If the
 * jsValue is not a object then a empty QBsonObject returned.
 *
 * @param jsValue
 *
 * @return QBsonObject
 */
QBsonObject QBsonConverter::convert(const QJSValue &jsValue)
{
    if (jsValue.isObject()) {
        return mapObject(jsValue);
    }
    return QBsonObject();
}

void QBsonConverter::setJSEngine(QJSEngine *engine)
{
    m_jsEngine = engine;
}

/**
 * @internal
 * @brief QBsonConverter::mapValue converts alls simple values and redirects to
 * mapObject and mapArray methods. If a value can not converted an
 * empty bsonvalue is returned.
 *
 * @param jsValue jsValue to convert
 *
 * @return QBsonValue
 */
QBsonValue QBsonConverter::mapValue(const QJSValue &jsValue)
{
    if (jsValue.isBool()) {
        return QBsonValue(jsValue.toBool());
    }
    if (jsValue.isDate()) {
        return QBsonValue(jsValue.toDateTime());
    }
    if (jsValue.isString()) {
        return QBsonValue(jsValue.toString());
    }
    if (jsValue.isNumber()) {
        int vType = jsValue.toVariant().type();
        switch (vType) {
            case QVariant::Double:
                return QBsonValue(jsValue.toVariant().toDouble());
                break;
            case QVariant::Int:
                return QBsonValue(jsValue.toVariant().toInt());
                break;
        }
    }
    if (jsValue.isArray()) {
        return mapArray(jsValue);
    }
    if (jsValue.isObject()) {
        return mapObject(jsValue);
    }

    return QBsonValue();
}

/**
 * @internal
 * @brief QBsonConverter::mapArray converts a jsArray to a bsonArray. Redirect
 * to mapValue method to convert array values.
 *
 * @param jsArray
 *
 * @return QBsonArray
 */
QBsonArray QBsonConverter::mapArray(const QJSValue &jsArray)
{
    QJSValueIterator it(jsArray);
    QBsonArray bsonArray;
    while (it.next()) {
        const QString name = it.name();
        if (name != QString::fromLatin1("length")) {
            bsonArray.append(mapValue(it.value()));
        }
    }
    return bsonArray;
}

/**
 * @internal
 * @brief QBsonConverter::mapObject converts a jsObject to a bsonObject. Redirect
 * to mapValue method to convert child values. Map property name id to _id.
 *
 * @param jsObject
 *
 * @return
 */
QBsonObject QBsonConverter::mapObject(const QJSValue &jsObject)
{
    QJSValueIterator it(jsObject);
    QBsonObject bsonObject;
    for (; it.next();) {
        const QString name = it.name();
        if (name == "id") {
            bsonObject.append("_id", QBsonOid(it.value().toString()));
            continue;
        }
        bsonObject.append(name, mapValue(it.value()));
    }
    return bsonObject;
}

/**
 * @internal
 * @brief QBsonConverter::mapValue map bson properties to jsvalue. Redirects
 * array and object to mapArray and mapObect methods.
 *
 * @param bsonValue
 *
 * @return QJSValue
 */
QJSValue QBsonConverter::mapValue(const QBsonValue &bsonValue)
{
    switch (bsonValue.type()) {
        case QBsonValue::Id:
        case QBsonValue::String:
            return QJSValue(bsonValue.toString());
        case QBsonValue::Long:
        case QBsonValue::Integer:
            return QJSValue(bsonValue.toInt());
        case QBsonValue::Double:
            return QJSValue(bsonValue.toDouble());
        case QBsonValue::Bool:
            return QJSValue(bsonValue.toBool());
        case QBsonValue::DateTime:
            return m_jsEngine->toScriptValue(bsonValue.toDateTime());
        case QBsonValue::Object:
            return mapObject(bsonValue.toObject());
        case QBsonValue::Array:
            return mapArray(bsonValue.toArray());
        default:;
    }
    return QJSValue();
}

/**
 * @internal
 * @brief QBsonConverter::mapArray map a array to QJSValue. Converts values with
 * mapValue method.
 *
 * @param bsonArray
 *
 * @return QJSValue
 */
QJSValue QBsonConverter::mapArray(const QBsonArray &bsonArray)
{
    QList<QBsonValue> arValues = bsonArray.values();
    QList<QBsonValue>::iterator it;
    QJSValue jsArray = m_jsEngine->newArray(bsonArray.size());
    int i = 0;
    for (it = arValues.begin(); it != arValues.end(); it++) {
        jsArray.setProperty(i++, mapValue(*it));
    }
    return jsArray;
}

/**
 * @internal
 * @brief QBsonConverter::mapObject
 * @param bsonObject
 * @return
 */
QJSValue QBsonConverter::mapObject(const QBsonObject &bsonObject)
{
    const QBsonValueHash values = bsonObject.values();
    QBsonValueHash::const_iterator it;
    QJSValue jsObject = m_jsEngine->newObject();
    for (it = values.begin(); it != values.end(); it++) {
        if (it.key() == "_id") {
            jsObject.setProperty("id", it.value().toString());
        }
        jsObject.setProperty(it.key(), mapValue(it.value()));
    }
    return jsObject;
}
