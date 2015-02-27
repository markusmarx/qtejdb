#include "tst_qbsonconverter.h"
#include "qbsonvalue.h"
#include <QTest>
#include <QJSEngine>
#include <QDateTime>


tst_QBsonConverter::tst_QBsonConverter(QObject *parent) : QObject(parent)
{

}

tst_QBsonConverter::~tst_QBsonConverter()
{

}

void tst_QBsonConverter::initTestCase()
{
    setJSEngine(new QJSEngine());
}

/**
 * @internal
 * @brief tst_QBsonConverter::tst_simplePropertyTypesFromJS convert from JS to Bson.
 */
void tst_QBsonConverter::tst_simplePropertyTypesFromJS()
{

    // bson to js
    QCOMPARE(mapValue(QJSValue("TestString")).toString(), QString("TestString"));
    QCOMPARE(mapValue(QJSValue(true)).toBool(), true);
    QCOMPARE(mapValue(QJSValue((int)12)).toInt(), 12);
    QCOMPARE(mapValue(QJSValue(12.3333)).toDouble(),
             12.3333);
    QDateTime currentDate;
    QJSEngine jsEngine;
    QCOMPARE(mapValue(jsEngine.toScriptValue(currentDate)).toDateTime(), currentDate);
    // array
    QJSValue jsValue = jsEngine.newArray();
    jsValue.setProperty(1, QJSValue("position1"));
    jsValue.setProperty(0, QJSValue("position0"));
    QBsonArray bsonArray = mapArray(jsValue);
    QCOMPARE(bsonArray.size(), 2);

    for (int i = 0; i < bsonArray.size(); i++) {
        QCOMPARE(bsonArray.value(i).toString(), QString("position%1").arg(i));
    }

    QBsonValue bsonValue = mapValue(jsValue);
    QVERIFY(bsonValue.isArray());

    // object
    jsValue = jsEngine.newObject();
    jsValue.setProperty("id", QJSValue("000000000000000000000000"));
    jsValue.setProperty("name1", QJSValue("position1"));
    jsValue.setProperty("name2", QJSValue("position0"));
    QBsonObject bsonObject = mapObject(jsValue);
    QCOMPARE(bsonObject.value("name1").toString(), QString("position1"));
    QCOMPARE(bsonObject.value("name2").toString(), QString("position0"));
    QCOMPARE(bsonObject.value("_id").toString(), QString("000000000000000000000000"));
    QVERIFY(bsonObject.value("_id").type() == QBsonValue::Id);

    bsonValue = mapValue(jsValue);
    QVERIFY(bsonValue.isObject());
}

/**
 * @brief tst_QBsonConverter::tst_simplePropertyTypesFromBson convert from Bson to JS.
 */
void tst_QBsonConverter::tst_simplePropertyTypesFromBson()
{

    QCOMPARE(mapValue(QBsonValue("TestString")).toString(), QString("TestString"));
    QCOMPARE(mapValue(QBsonValue(true)).toBool(), true);
    QCOMPARE(mapValue(QBsonValue((int)12)).toInt(), 12);
    QCOMPARE(mapValue(QBsonValue(12.3333)).toNumber(), 12.3333);
    QDateTime currentDate;
    QCOMPARE(mapValue(QBsonValue(currentDate)).toDateTime(), currentDate);

    QBsonArray bsArray;
    bsArray.append(QBsonValue("position0"));
    bsArray.append(QBsonValue("position1"));
    QJSValue jsArray = mapArray(bsArray);
    QCOMPARE(jsArray.property("length").toInt(), 2);
    for (int i = 0; i < bsArray.size(); i++) {
        QCOMPARE(jsArray.property(i).toString(), QString("position%1").arg(i));
    }

    QBsonObject bsonObject;
    bsonObject.append("name0", "position0");
    bsonObject.append("name1", "position1");
    bsonObject.append("_id", QBsonOid());
    QJSValue jsObject = mapObject(bsonObject);
    QCOMPARE(jsObject.property("name0").toString(), QString("position0"));
    QCOMPARE(jsObject.property("name1").toString(), QString("position1"));
    QCOMPARE(jsObject.property("id").toString(), QString("000000000000000000000000"));
}

QTEST_MAIN(tst_QBsonConverter)

