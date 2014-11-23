#include "qejdbdatabaseservice.h"
#include <QDebug>
#include "serverconfiguration.h"
#include <QCoreApplication>

QTRPC_REGISTER_METATYPE(QEjdbDatabaseService::ResultData)

QTRPC_REGISTER_METATYPE(QBsonObject)

QEjdbDatabaseService::QEjdbDatabaseService(QObject *parent) :
    ServiceProxy(parent)
{
    ServerConfiguration *config = qApp->findChild<ServerConfiguration*>("serverConfig");
    m_database = QEjdbDatabase::database(config->databaseName());
}


ReturnValue QEjdbDatabaseService::auth(QString user, QString passwd)
{
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


QDataStream &operator<<(QDataStream &d, const QEjdbDatabaseService::ResultData &object)
{
    d << object.count;
    d << object.result;
}


QDataStream &operator>>(QDataStream &d, QEjdbDatabaseService::ResultData &object)
{
    d >> object.count;
    d >> object.result;
}
