#include "qejdbclient.h"
#include <QJSValue>
#include <QDebug>
#include <QUuid>
#include <QQmlEngine>
#include <QQmlContext>
#include "qejdbdatabase.h"
#include "qejdbexception.h"

/**
 * @class QEjdbClient
 * @ingroup QtQuick
 * @brief QEjdbClient handles all communication with the QEjdb database.
 *
 * @section mapping How Simple Properties are mapped from QtQuick
 *
 * The id property name is renamed to _id in the bson representation and
 * visa versa. Id value must be a 12 byte array QBsonOid .
 *
 *  QBsonValue   | QJsValue
 * ------------- | -------------
 * Id            | String
 * String        | String
 * Double        | Double
 * Integer       | Integer
 * Long          | Integer
 * Bool          | Bool
 * DateTime      | DateTime
 *
 */


QEjdbClient::QEjdbClient(QObject *parent) : QObject(parent)
{

}

QEjdbClient::~QEjdbClient()
{
    disconnect();
}

/**
 * @brief QEjdbClient::classBegin
 */
void QEjdbClient::classBegin()
{

}

void QEjdbClient::componentComplete()
{

    if (m_connectionName.isEmpty()) {
        m_connectionName = QUuid::createUuid().toString();
    }

    QQmlEngine *engine = QQmlEngine::contextForObject(this)->engine();
    m_bsonConverter.setJSEngine(engine);
}

QJSValue QEjdbClient::save(QString collectionName, const QJSValue &jsValue)
{
    qDebug() << "save value" << jsValue.toVariant() << "in" << collectionName;

    QBsonObject bsonObj = convert(jsValue);

    QEjdbDatabase db = database();
    checkCollection(db, collectionName);
    db.save(collectionName, bsonObj);

    QJSValue resultJs(jsValue);
    resultJs.setProperty("id", QJSValue(bsonObj.value("_id").toString()));
    return resultJs;
}

QJSValue QEjdbClient::load(QString collectionName, QJSValue uid)
{
    QEjdbDatabase db = database();
    checkCollection(db, collectionName);
    return QJSValue(convert(db.load(collectionName, uid.toString())));
}

QString QEjdbClient::uri() const
{
    return m_uri;
}

QString QEjdbClient::connectionName() const
{
    return m_connectionName;
}

bool QEjdbClient::autoCreateCollection() const
{
    return m_autoCreateCollection;
}

void QEjdbClient::connect()
{
    qDebug() << "connect QEjdb on" << m_uri
             << "and connectioName" << m_connectionName;

    QEjdbDatabase db;

    db = QEjdbDatabase::addDatabase(m_uri, m_connectionName);
    db.open();

}

void QEjdbClient::disconnect()
{
    QEjdbDatabase::removeDatabase(m_connectionName);
}

void QEjdbClient::setConnectionName(QString arg)
{
    if (m_connectionName == arg)
        return;

    m_connectionName = arg;
    emit connectionNameChanged(arg);
}

void QEjdbClient::setUri(QString arg)
{
    if (m_uri == arg)
        return;

    m_uri = arg;
    emit uriChanged(arg);
}


void QEjdbClient::setAutoCreateCollection(bool arg)
{
    if (m_autoCreateCollection == arg)
        return;

    m_autoCreateCollection = arg;
    emit autoCreateCollectionChanged(arg);
}

/**
 * @internal
 * @brief QEjdbClient::convert convert a QJsValue to QBsonObject. See @ref mapping
 * how to property types are mapped.
 *
 * @param jsValue a QJSValue instance
 *
 * @return QBsonObject converted instance
 */
QBsonObject QEjdbClient::convert(const QJSValue &jsValue)
{
    return m_bsonConverter.convert(jsValue);
}

/**
 * @internal
 * @brief QEjdbClient::convert convert a QBsonValue to QJsValue. See @ref mapping
 * how to property types are mapped.
 *
 * @param bsonObject a QBsonObject to convert
 *
 * @return QJSValue converted value
 */
QJSValue QEjdbClient::convert(const QBsonObject &bsonObject)
{
    return m_bsonConverter.convert(bsonObject);
}
