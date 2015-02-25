#include "qejdbclient.h"
#include <QJSValue>
#include <QDebug>
#include <QUuid>
#include "qejdbdatabase.h"
#include "qejdbexception.h"

QEjdbClient::QEjdbClient(QObject *parent) : QObject(parent)
{

}

QEjdbClient::~QEjdbClient()
{

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

}

QJSValue QEjdbClient::save(QString collectionName, const QJSValue &jsValue)
{
    qDebug() << "save value" << jsValue.property("date").toVariant() << "in" << collectionName;

    QBsonObject bsonObj;// = QEjdbQmlConverter::convert(jsValue);
    bsonObj.append("name", "test");
    QEjdbDatabase db = database();
    checkCollection(db, collectionName);
    db.save(collectionName, bsonObj);

    QJSValue resultJs(jsValue);
    resultJs.setProperty("id", QJSValue(bsonObj.value("_id").toString()));
    return resultJs;
}

QJSValue QEjdbClient::load(QString collectionName, QString uid)
{

}

QString QEjdbClient::uri() const
{
    return m_uri;
}

QString QEjdbClient::connectionName() const
{
    return m_connectionName;
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

