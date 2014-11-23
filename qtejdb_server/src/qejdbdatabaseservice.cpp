#include "qejdbdatabaseservice.h"
#include <QDebug>
#include "serverconfiguration.h"
#include <QCoreApplication>

QTRPC_REGISTER_METATYPE(QEjdbDatabaseService::ResultData)
QTRPC_REGISTER_METATYPE(QBsonObject)
QTRPC_REGISTER_METATYPE(QList<QBsonObject>)

QEjdbDatabaseService::QEjdbDatabaseService(QObject *parent) :
    ServiceProxy(parent)
{
    ServerConfiguration *config = qApp->findChild<ServerConfiguration*>("serverConfig");
    m_database = QEjdbDatabase::database(config->databaseName());
}


ReturnValue QEjdbDatabaseService::auth(QString user, QString passwd)
{
    Q_UNUSED(user);Q_UNUSED(passwd);
    return ReturnValue(true);
}

ReturnValue QEjdbDatabaseService::save(QString collectionName, QBsonObject obj)
{
    qDebug() << "save bson";
    m_database.save(collectionName, obj);
    return ReturnValue(obj.value("_id").toString());
}

ReturnValue QEjdbDatabaseService::createCollection(QString collectionName)
{
    qDebug() << "create collection";
    return ReturnValue(m_database.createCollection(collectionName));
}

ReturnValue QEjdbDatabaseService::removeCollection(QString collectionName)
{
    return ReturnValue(m_database.removeCollection(collectionName));
}

ReturnValue QEjdbDatabaseService::containsCollection(QString collectionName)
{
    return ReturnValue(m_database.containsCollection(collectionName));
}


/**
 * @brief QEjdbDatabaseService::load Load a BsonObject by id.
 *
 * @param collectionName name of collection
 * @param oid id of object
 *
 * @return ReturnValue
 */
ReturnValue QEjdbDatabaseService::load(const QString &collectionName, const QString &oid)
{
    QBsonObject obj = m_database.load(collectionName, oid);
    return ReturnValue(QVariant::fromValue(obj));
}

/**
 * @brief QEjdbDatabaseService::remove Remove a bsonobject by id.
 *
 * @param collectionName name of collection where the bson is stored.
 * @param oid id of bson.
 *
 * @return true if bson was deleted else false.
 */
ReturnValue QEjdbDatabaseService::remove(const QString &collectionName, const QString &oid)
{
    qDebug() << "collectionName:" << collectionName << ", oid:" << oid;
    bool ret = m_database.remove(collectionName, oid);
    return ReturnValue(ret);
}

ReturnValue QEjdbDatabaseService::query(const QString &collectionName, const QBsonObject &query)
{
    qDebug() << "collectionName:" << collectionName << ", query:" << query;
    QList<QBsonObject> resultList = m_database.query(collectionName, query);
    qDebug() << "found" << resultList.size() << "items";
    return ReturnValue(QVariant::fromValue(resultList));
}


QDataStream &operator<<(QDataStream &d, const QEjdbDatabaseService::ResultData &object)
{
    d << object.count;
    d << object.result;
    return d;
}


QDataStream &operator>>(QDataStream &d, QEjdbDatabaseService::ResultData &object)
{
    d >> object.count;
    d >> object.result;
    return d;
}
