#include "qejdbclient.h"
#include "qejdbclient_p.h"
#include <QJSValue>
#include <QDebug>
#include <QUuid>
#include <QQmlEngine>
#include <QQmlContext>
#include "qejdbdatabase.h"
#include "qejdbexception.h"
#include "QtQml/QtQml"

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

QJSValue QEjdbClientPrivate::save(QString collectionName, const QJSValue &jsValue)
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

QJSValue QEjdbClientPrivate::load(QString collectionName, QJSValue uid)
{
    QEjdbDatabase db = database();
    checkCollection(db, collectionName);
    return QJSValue(convert(db.load(collectionName, uid.toString())));
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
QBsonObject QEjdbClientPrivate::convert(const QJSValue &jsValue)
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
QJSValue QEjdbClientPrivate::convert(const QBsonObject &bsonObject)
{
    return m_bsonConverter.convert(bsonObject);
}


QEjdbClient::QEjdbClient(QObject *parent)
    : QObject(parent), d_ptr(new QEjdbClientPrivate(this))
{

}

QEjdbClient::~QEjdbClient()
{
    disconnect();
    delete d_ptr;
}

/**
 * @brief QEjdbClient::classBegin
 */
void QEjdbClient::classBegin()
{

}

void QEjdbClient::componentComplete()
{
    Q_D(QEjdbClient);
    if (d->m_connectionName.isEmpty()) {
        d->m_connectionName = QUuid::createUuid().toString();
    }

    QJSEngine *engine = QtQml::qmlEngine(this);
    d->m_bsonConverter.setJSEngine(engine);
}

/**
 * @brief QEjdbClient::save save the json in the collection.
 *
 * @param collectionName
 *
 * @param jsValue QJSValue to save
 *
 * @return
 */
QJSValue QEjdbClient::save(QString collectionName, const QJSValue &jsValue)
{
    Q_D(QEjdbClient);
    return d->save(collectionName, jsValue);
}

QJSValue QEjdbClient::load(QString collectionName, QJSValue uid)
{
    Q_D(QEjdbClient);
    return d->load(collectionName, uid);
}


QString QEjdbClient::uri() const
{

    return d_ptr->m_uri;
}

QString QEjdbClient::connectionName() const
{
    return d_ptr->m_connectionName;
}

bool QEjdbClient::autoCreateCollection() const
{

    return d_ptr->m_autoCreateCollection;
}

void QEjdbClient::connect()
{
    Q_D(QEjdbClient);
    qDebug() << "connect QEjdb on" << d->m_uri
             << "and connectioName" << d->m_connectionName;

    QEjdbDatabase db;

    db = QEjdbDatabase::addDatabase(d->m_uri, d->m_connectionName);
    db.open();

}

void QEjdbClient::disconnect()
{
    Q_D(QEjdbClient);
    QEjdbDatabase::removeDatabase(d->m_connectionName);
}

void QEjdbClient::setConnectionName(QString arg)
{
    Q_D(QEjdbClient);
    if (d->m_connectionName == arg)
        return;

    d->m_connectionName = arg;
    emit connectionNameChanged(arg);
}

void QEjdbClient::setUri(QString arg)
{
    Q_D(QEjdbClient);
    if (d->m_uri == arg)
        return;

    d->m_uri = arg;
    emit uriChanged(arg);
}


void QEjdbClient::setAutoCreateCollection(bool arg)
{
    Q_D(QEjdbClient);
    if (d->m_autoCreateCollection == arg)
        return;

    d->m_autoCreateCollection = arg;
    emit autoCreateCollectionChanged(arg);
}
