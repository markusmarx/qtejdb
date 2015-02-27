#ifndef QBSONCONVERTER_H
#define QBSONCONVERTER_H

#include <QJSValue>
#include <QJSEngine>
#include "qbsonobject.h"
#include "qbsonarray.h"

class QBsonConverter
{
public:
    QBsonConverter();
    ~QBsonConverter();

    QJSValue convert(const QBsonObject &bsonObject);
    QBsonObject convert(const QJSValue &jsValue);
    void setJSEngine(QJSEngine *engine);
protected:

    QBsonValue mapValue(const QJSValue &jsValue);
    QJSValue mapValue(const QBsonValue &bsonValue);
    QBsonArray mapArray(const QJSValue &jsArray);
    QJSValue mapArray(const QBsonArray &bsonArray);
    QBsonObject mapObject(const QJSValue &jsObject);
    QJSValue mapObject(const QBsonObject &bsonObject);

private:

    /**
     * @internal
     * @brief m_jsEngine reference to jsEngine to create Array and Object values.
     */
    QJSEngine *m_jsEngine;
};

#endif // QBSONCONVERTER_H
