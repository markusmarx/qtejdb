#include "qejdbclient.h"
#include "qejdbclient_p.h"
#include <QJSValue>
#include <QDebug>
#include <QUuid>
#include <QQmlEngine>
#include <QQmlContext>
#include "qtejdb/qejdbdatabase.h"
#include "qtejdb/qejdbexception.h"
#include "basemodel.h"
#include "qbson/qbsonvalue.h"
#include <QtQml/qqml.h>

/*!
  \qmltype QEjdbClient
  \since QtEjdb 1.0
  \inqmlmodule QtEjdb
  \ingroup qtejdb-qml
  \target QEjdbClientQml
  \brief Client interface to access ejdb service.
    \snippet simple.qml import
    QEjdbClient connects an ejdb.
    It is used for all communication with the QtEjdb backend.
    \snippet simple.qml client

    An client can connect an ejdb with an \l uri or a connectionName. With a
    connectionName you can connect an previous opened connection.
    \code
    // c++
    QEjdbDatabase::addDatabase("file:test_db", "myconnection");
    // qml
    QEjdbClient {
        id: client
        connectionName: 'myconnection'
    }
    \endcode
*/

/*!
  \qmlproperty QString QEjdbClient::uri

  Open a database from uri.
  \snippet simple.qml client

 */
/*!
  \qmlproperty QString QEjdbClient::connectionName

  Open a database from an already opened QEjdbDatabase.
 */

/*!
  \qmlmethod QJsValue QEjdbClient::save(QString collectionName, QJSValue json)
  Saves a bson in a given collection. The returned json have a _id property
  with a generated bson id.
  \snippet simple.qml save
 */
/*!
 * \qmlmethod QJSValue QEjdbClient::load(QString collectionName, QJsValue uid)
 * Loads a bson with his id. If no bson found an empty json is returned.
 */
/*!
 * \qmlmethod void QEjdbClient::createCollection(QString collectionName);
 * Creates a collection with name.
 */
/*!
 * \qmlmethod void QEjdbClient::removeCollection(QString collectionName);
 * Removes a collection with name.
 */
/*!
 * \qmlmethod void QEjdbClient::remove(QString collectionName, QJsValue id);
 * Removes a json with  from collection.
 */
/*!
 * \qmlmethod void QEjdbClient::query(QString collectionName, QJsValue query, QJsValue hints);
 * Get an array with json from collection. Desciption about building queries and hints
 * see \l http://ejdb.org/doc/ql/ql.html
 */
QJSValue QEjdbClientPrivate::save(QString collectionName, const QJSValue &jsValue)
{
    qDebug() << "save value" << jsValue.toVariant() << "in" << collectionName;

    QBsonObject bsonObj = convert(jsValue).toObject();

    QEjdbDatabase db = database();
    checkCollection(db, collectionName);
    db.save(collectionName, bsonObj);

    QJSValue resultJs(jsValue);
    resultJs.setProperty("_id", QJSValue(bsonObj.oid().toString()));

    return resultJs;
}

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

QJSValue QEjdbClientPrivate::query(QString collectionName, QJSValue query, QJSValue hints)
{
    QEjdbDatabase db = database();
    QBsonObject queryObj = convert(query).toObject();
    QBsonObject hintsObj = convert(hints).toObject();
    QEjdbResult res = db.query(collectionName, queryObj, hintsObj);
    return m_bsonConverter.convert(res.values());
}


QBsonValue QEjdbClientPrivate::convert(const QJSValue &jsValue)
{
    return m_bsonConverter.convert(jsValue);
}

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
    delete d_ptr;
}

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

    QJSEngine *engine = qmlEngine(this);

    if (engine == 0) {
        engine = new QJSEngine(this);
    }

    d->m_bsonConverter.setJSEngine(engine);
    connect();
}

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

QJSValue QEjdbClient::query(QString collectionName, QJSValue query, QJSValue hints)
{
    Q_D(QEjdbClient);
    return d->query(collectionName, query, hints);
}

void QEjdbClient::createCollection(QString collection)
{
    Q_D(QEjdbClient);
    d->database().createCollection(collection);
}

void QEjdbClient::removeCollection(QString collection)
{
    Q_D(QEjdbClient);
    d->database().dropCollection(collection);
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

QBsonValue QEjdbClient::convert(const QJSValue &jsValue)
{
    return d_ptr->convert(jsValue);
}

QJSValue QEjdbClient::convert(const QBsonObject &bsonObject)
{
    return d_ptr->convert(bsonObject);
}

bool QEjdbClient::isConnected()
{
    return d_ptr->m_isConnected;
}

void QEjdbClient::registerModel(BaseModel *baseModel)
{
    if (isConnected()) {
        baseModel->connected();
    }
    QObject::connect(this, &QEjdbClient::connected, baseModel, &BaseModel::connected);
    QObject::connect(this, &QEjdbClient::disconnected, baseModel, &BaseModel::disconnected);
}

void QEjdbClient::connect()
{
    Q_D(QEjdbClient);

    if (d->m_isConnected) {
        return;
    }

    qWarning() << "connect QEjdb on" << d->m_uri
             << "and connectioName" << d->m_connectionName;

    QEjdbDatabase db;
    QString uri = d->m_uri;
    QString connectionName = d->m_connectionName;
    if (!uri.isEmpty()) {
        try {
            db = QEjdbDatabase::database(connectionName);
        } catch(QEjdbException e) {
            db = QEjdbDatabase::addDatabase(uri, connectionName);
        }
    } else {
        db = QEjdbDatabase::database(connectionName);
    }
    d->m_isConnected = true;
    emit connected();
}

void QEjdbClient::disconnect()
{
    QEjdbDatabase db = d_ptr->database();
    db.close();
    QEjdbDatabase::removeDatabase(d_ptr->m_connectionName);
}

void QEjdbClient::setConnectionName(QString connectionName)
{
    Q_D(QEjdbClient);
    if (d->m_connectionName == connectionName)
        return;

    d->m_connectionName = connectionName;
    emit connectionNameChanged(connectionName);
}

void QEjdbClient::setUri(QString uri)
{
    Q_D(QEjdbClient);
    if (d->m_uri == uri)
        return;

    d->m_uri = uri;
    emit uriChanged(uri);
}

void QEjdbClient::setAutoCreateCollection(bool autoCreateCollection)
{
    Q_D(QEjdbClient);
    if (d->m_autoCreateCollection == autoCreateCollection)
        return;

    d->m_autoCreateCollection = autoCreateCollection;
    emit autoCreateCollectionChanged(autoCreateCollection);
}
