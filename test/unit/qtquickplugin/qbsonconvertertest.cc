#include "gtest/gtest.h"
#include "qtquickplugin/qbsonconverter.h"
#include "../helper.h"
#include <QJSValue>
#include <QJSEngine>
#include <QJsonObject>

TEST(QBsonConverterTest, TestCovertQBsonObjectWithInt)
{
    QBsonConverter *conv = new QBsonConverter;
    conv->setJSEngine(new QJSEngine);
    QBsonObject obj("number", 11);
    QJSValue jsVal = conv->convert(obj);
    EXPECT_EQ(11, jsVal.property("number").toInt());
    delete conv;
}
TEST(QBsonConverterTest, TestCovertQBsonObjectWithDouble)
{
    QBsonConverter *conv = new QBsonConverter;
    conv->setJSEngine(new QJSEngine);
    QBsonObject obj("number", 11.11);
    QJSValue jsVal = conv->convert(obj);
    EXPECT_EQ(11.11, jsVal.property("number").toNumber());
    delete conv;
}

TEST(QBsonConverterTest, TestCovertQBsonObjectWithString)
{
    QBsonConverter *conv = new QBsonConverter;
    conv->setJSEngine(new QJSEngine);
    QBsonObject obj("string", "test");
    QJSValue jsVal = conv->convert(obj);
    EXPECT_EQ("test", jsVal.property("string").toString());
    delete conv;
}
TEST(QBsonConverterTest, TestCovertQBsonObjectWithBool)
{
    QBsonConverter *conv = new QBsonConverter;
    conv->setJSEngine(new QJSEngine);
    QBsonObject obj("bool", true);
    QJSValue jsVal = conv->convert(obj);
    EXPECT_EQ(true, jsVal.property("bool").toBool());
    delete conv;
}
TEST(QBsonConverterTest, TestCovertQBsonObjectWithDatetime)
{
    QBsonConverter *conv = new QBsonConverter;
    conv->setJSEngine(new QJSEngine);
    QDateTime now = QDateTime::currentDateTime();
    QBsonObject obj("datetime", now);
    QJSValue jsVal = conv->convert(obj);
    EXPECT_EQ(now, jsVal.property("datetime").toDateTime());
    delete conv;
}
TEST(QBsonConverterTest, TestCovertQBsonObjectWithSubObject)
{
    QBsonConverter *conv = new QBsonConverter;
    conv->setJSEngine(new QJSEngine);
    QBsonObject obj("object", QBsonObject("subobj", "test"));
    QJSValue jsVal = conv->convert(obj);
    EXPECT_TRUE(jsVal.property("object").isObject());
    EXPECT_EQ("test", jsVal.property("object").property("subobj").toString());
    delete conv;
}
TEST(QBsonConverterTest, TestCovertQBsonObjectWithArray)
{
    QBsonConverter *conv = new QBsonConverter;
    conv->setJSEngine(new QJSEngine);
    QBsonArray array;
    array.append(1).append(2);
    QBsonObject obj("array", array);
    QJSValue jsVal = conv->convert(obj);
    EXPECT_TRUE(jsVal.property("array").isArray());
    EXPECT_EQ(1, jsVal.property("array").property(0).toInt());
    EXPECT_EQ(2, jsVal.property("array").property(1).toInt());
    delete conv;
}
TEST(QBsonConverterTest, TestCovertQBsonObjectWithId)
{
    QBsonConverter *conv = new QBsonConverter;
    conv->setJSEngine(new QJSEngine);
    QBsonObject obj;
    obj.setOid(QBsonOid::generate());
    QJSValue jsVal = conv->convert(obj);
    EXPECT_TRUE(jsVal.hasProperty("_id"));
    EXPECT_EQ(obj.oid().toString(), jsVal.property("_id").toString());
    delete conv;
}
TEST(QBsonConverterTest, TestConvertQJsValueWithInt)
{
    QJSEngine *jsEngine = new QJSEngine;
    QBsonConverter *conv = new QBsonConverter;
    conv->setJSEngine(jsEngine);
    QJSValue obj = jsEngine->newObject();
    obj.setProperty("number", 11);
    QBsonObject bObj = conv->convert(obj);
    EXPECT_EQ(11, bObj.value("number").toInt());
    delete conv;
    delete jsEngine;
}
TEST(QBsonConverterTest, TestConvertQJsValueWithDouble)
{
    QJSEngine *jsEngine = new QJSEngine;
    QBsonConverter *conv = new QBsonConverter;
    conv->setJSEngine(jsEngine);
    QJSValue obj = jsEngine->newObject();
    obj.setProperty("number", 11.11);
    QBsonObject bObj = conv->convert(obj);
    EXPECT_EQ(11.11, bObj.value("number").toDouble());
    delete conv;
    delete jsEngine;
}
TEST(QBsonConverterTest, TestConvertQJsValueWithString)
{
    QJSEngine *jsEngine = new QJSEngine;
    QBsonConverter *conv = new QBsonConverter;
    conv->setJSEngine(jsEngine);
    QJSValue obj = jsEngine->newObject();
    obj.setProperty("string", "test");
    QBsonObject bObj = conv->convert(obj);
    EXPECT_EQ("test", bObj.value("string").toString());
    delete conv;
    delete jsEngine;
}
TEST(QBsonConverterTest, TestConvertQJsValueWithBool)
{
    QJSEngine *jsEngine = new QJSEngine;
    QBsonConverter *conv = new QBsonConverter;
    conv->setJSEngine(jsEngine);
    QJSValue obj = jsEngine->newObject();
    obj.setProperty("bool", true);
    QBsonObject bObj = conv->convert(obj);
    EXPECT_EQ(true, bObj.value("bool").toBool());
    delete conv;
    delete jsEngine;
}
TEST(QBsonConverterTest, TestConvertQJsValueWithDatetime)
{
    QJSEngine *jsEngine = new QJSEngine;
    QBsonConverter *conv = new QBsonConverter;
    conv->setJSEngine(jsEngine);
    QJSValue obj = jsEngine->newObject();
    QDateTime now = QDateTime::currentDateTime();
    obj.setProperty("datetime", jsEngine->toScriptValue(now));
    QBsonObject bObj = conv->convert(obj);
    EXPECT_EQ(now, bObj.value("datetime").toDateTime());
    delete conv;
    delete jsEngine;
}
TEST(QBsonConverterTest, TestConvertQJsValueWithSubObject)
{
    QJSEngine *jsEngine = new QJSEngine;
    QBsonConverter *conv = new QBsonConverter;
    conv->setJSEngine(jsEngine);
    QJSValue obj = jsEngine->newObject();
    QJSValue subobj = jsEngine->newObject();
    subobj.setProperty("name", "subobj");
    obj.setProperty("subobj", subobj);
    QBsonObject bObj = conv->convert(obj);
    EXPECT_EQ("subobj", bObj.value("subobj").toObject().value("name"));
    delete conv;
    delete jsEngine;
}
TEST(QBsonConverterTest, TestConvertQJsValueWithArray)
{
    QJSEngine *jsEngine = new QJSEngine;
    QBsonConverter *conv = new QBsonConverter;
    conv->setJSEngine(jsEngine);
    QJSValue obj = jsEngine->newObject();
    QJSValue array = jsEngine->newArray();
    array.setProperty(0, 1);
    array.setProperty(1, 2);
    obj.setProperty("array", array);
    QBsonObject bObj = conv->convert(obj);
    EXPECT_EQ(1, bObj.value("array").toArray().value(0).toInt());
    EXPECT_EQ(2, bObj.value("array").toArray().value(1).toInt());
    delete conv;
    delete jsEngine;
}
TEST(QBsonConverterTest, TestConvertQJsValueWithId)
{
    QJSEngine *jsEngine = new QJSEngine;
    QBsonConverter *conv = new QBsonConverter;
    conv->setJSEngine(jsEngine);
    QJSValue obj = jsEngine->newObject();
    QBsonOid oid = QBsonOid::generate();
    obj.setProperty("_id", oid.toString());
    QBsonObject bObj = conv->convert(obj);
    EXPECT_EQ(oid, bObj.oid());
    delete conv;
    delete jsEngine;
}
