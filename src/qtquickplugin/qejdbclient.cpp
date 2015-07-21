#include "qejdbclient.h"
#include "qejdbclient_p.h"
#include <QJSValue>
#include <QDebug>
#include <QUuid>
#include <QQmlEngine>
#include <QQmlContext>
#include "qtejdb/qejdbdatabase.h"
#include "qtejdb/qejdbexception.h"
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
 *
 *
 */

/**
 * @brief QEjdbClientPrivate::save Saves a Json in database. If autoCreateCollection is true
 * colleciton will created.
 *
 * @param collectionName name of collection
 *
 * @param jsValue
 *
 * @return created Json with id parameter
 */
QJSValue QEjdbClientPrivate::save(QString collectionName, const QJSValue &jsValue)
{
    qDebug() << "save value" << jsValue.toVariant() << "in" << collectionName;

    QBsonObject bsonObj = convert(jsValue);

    QEjdbDatabase db = database();
    checkCollection(db, collectionName);
    db.save(collectionName, bsonObj);

    QJSValue resultJs(jsValue);
    resultJs.setProperty("_id", QJSValue(bsonObj.oid().toString()));

    return resultJs;
}

/**
 * @brief QEjdbClientPrivate::load load a Json from database stored in collection
 * identified by collectionName. If nothing found en empty Json is returned.
 *
 * @param collectionName name of collection
 * @param uid Bson uid
 *
 * @return Json Object or empty Json if noting found
 */
QJSValue QEjdbClientPrivate::load(QString collectionName, QJSValue uid)
{
    QEjdbDatabase db = database();
    if (db.containsCollection(collectionName)) {
        QBsonOid oid = QBsonOid(uid.toString());
        if (oid.isValid()) {
            QBsonObject bso = db.load(collectionName, uid.toString());
            if (bso.isEmpty()) {
                return QJSValue(QJSValue::NullValue);
            }
            return QJSValue(convert(bso));
        }
    }
    return QJSValue(QJSValue::NullValue);
}

/**
 * @brief QEjdbClientPrivate::remove remove a bson from database collection. Returns true
 * if remove succeed otherwise false.
 *
 */
QJSValue QEjdbClientPrivate::remove(QString collectionName, QJSValue uid)
{
    QEjdbDatabase db = database();
    if (db.containsCollection(collectionName)) {
        QBsonOid oid = QBsonOid(uid.toString());
        if (oid.isValid()) {
            return QJSValue(db.remove(collectionName, uid.toString()));
        }
    }
    return QJSValue(false);
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

/**
 * @brief QEjdbClient::QEjdbClient
 * @param parent
 */
QEjdbClient::QEjdbClient(QObject *parent)
    : QObject(parent), d_ptr(new QEjdbClientPrivate(this))
{

}

/**
 * @brief QEjdbClient::~QEjdbClient delete object
 */
QEjdbClient::~QEjdbClient()
{
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
    d->m_autoCloseConnection = false;
    d->m_autoCreateCollection = true;
    if (d->m_connectionName.isEmpty()) {
        d->m_connectionName = QUuid::createUuid().toString();
        d->m_autoCloseConnection = true;
    }

    QJSEngine *engine = QtQml::qmlEngine(this);

    if (engine == 0) {
        engine = new QJSEngine(this);
    }

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

QJSValue QEjdbClient::remove(QString collectionName, QJSValue uid)
{
    Q_D(QEjdbClient);
    return d->remove(collectionName, uid);
}

void QEjdbClient::createCollection(QString collection)
{
    Q_D(QEjdbClient);
    d->database().createCollection(collection);
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

/**
 * @brief QEjdbClient::autoCreateCollection If true a collection that not exist
 * is created automatically on load, save or query methods.
 *
 * @return value of autoCreateCollection
 */
bool QEjdbClient::autoCreateCollection() const
{

    return d_ptr->m_autoCreateCollection;
}

/**
 * @brief QEjdbClient::convert tranform QJSValue to QBsonObject.
 */
QBsonObject QEjdbClient::convert(const QJSValue &jsValue)
{
    return d_ptr->convert(jsValue);
}
/**
 * @brief QEjdbClient::convert tranform QBsonObject to QJSValue.
 */
QJSValue QEjdbClient::convert(const QBsonObject &bsonObject)
{
    return d_ptr->convert(bsonObject);
}

bool QEjdbClient::isConnected()
{
    return d_ptr->m_isConnected;
}

/**
 * @brief QEjdbClient::connect
 */
void QEjdbClient::connect()
{
    Q_D(QEjdbClient);
    qWarning() << "connect QEjdb on" << d->m_uri
             << "and connectioName" << d->m_connectionName;

    QEjdbDatabase db;
    QString uri = d->m_uri;
    QString connectionName = d->m_connectionName;

    db = QEjdbDatabase::addDatabase(uri, connectionName);
    d->m_isConnected = true;
    emit connected();
}

/**
 * @brief QEjdbClient::disconnect close database connection and remove database
 * from dictionary.
 *
 * @see QEjdbDatabase::removeDatabase()
 */
void QEjdbClient::disconnect()
{
    QEjdbDatabase db = d_ptr->database();
    db.close();
    QEjdbDatabase::removeDatabase(d_ptr->m_connectionName);
}

/**
 * @brief QEjdbClient::setConnectionName set connection name that is stored in
 * database connection dictionary
 *
 * @param connectionName name of connection
 *
 * @return void
 *
 * @see QEjdbDatabase::addDatabase()
 */
void QEjdbClient::setConnectionName(QString connectionName)
{
    Q_D(QEjdbClient);
    if (d->m_connectionName == connectionName)
        return;

    d->m_connectionName = connectionName;
    emit connectionNameChanged(connectionName);
}

/**
 * @brief QEjdbClient::setUri set uri to connect database.
 *
 * @param uri connection uri
 *
 * @return void
 *
 * @see QEjdbDatabase::addDatabase(QString,QString)
 */
void QEjdbClient::setUri(QString uri)
{
    Q_D(QEjdbClient);
    if (d->m_uri == uri)
        return;

    d->m_uri = uri;
    emit uriChanged(uri);
}

/**
 * @brief QEjdbClient::setAutoCreateCollection If true a collection that not
 * exist is created automaticaly.
 *
 * @param autoCreateCollection true creates a collection or false not.
 *
 * @return void
 *
 * @see QEjdbClient::autoCreateCollection()
 */
void QEjdbClient::setAutoCreateCollection(bool autoCreateCollection)
{
    Q_D(QEjdbClient);
    if (d->m_autoCreateCollection == autoCreateCollection)
        return;

    d->m_autoCreateCollection = autoCreateCollection;
    emit autoCreateCollectionChanged(autoCreateCollection);
}
