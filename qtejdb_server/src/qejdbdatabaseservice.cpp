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
    m_database.save(collectionName, obj);
    return ReturnValue(true);
}

ReturnValue QEjdbDatabaseService::createCollection(QString collectionName)
{
    return ReturnValue(m_database.createCollection(collectionName));
}

ReturnValue QEjdbDatabaseService::removeCollection(QString collectionName)
{
    return ReturnValue(m_database.removeCollection(collectionName));
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
