#include "qejdbrpcworker.h"
#include "qejdbexception.h"
#include <QDir>

QEjdbRpcWorker::QEjdbRpcWorker(const QUrl &url, int mode)
    :m_url(url), m_clientService(new QEjdbRpcClientService())
{
}

void QEjdbRpcWorker::open()
{

    ReturnValue ret;

    // scheme is: socket:[socketname]
    if (m_url.scheme() == QStringLiteral("socket")) {
        ret = m_clientService->connect("socket:" + QDir::temp().absoluteFilePath(m_url.path()));
        if (ret.toBool()) m_clientService->selectService(m_url.path());
    }

    if (ret.isError()) {
        throw QEjdbException(QEjdbException::CONNECTIONERROR, "could not open database connection on " + m_url.toString());
    }

}

bool QEjdbRpcWorker::isOpen()
{
    return m_clientService->state() == QEjdbRpcClientService::Connected;
}

bool QEjdbRpcWorker::close()
{
    m_clientService->disconnect();
}

bool QEjdbRpcWorker::containsCollection(const QString &collectionName)
{
    ReturnValue ret = m_clientService->containsCollection(collectionName);
    checkReturnValue(ret);
    return ret.toBool();
}

bool QEjdbRpcWorker::createCollection(const QString &collectionName)
{
    ReturnValue ret = m_clientService->createCollection(collectionName).toBool();
    checkReturnValue(ret);
    return ret.toBool();
}

bool QEjdbRpcWorker::removeCollection(const QString &collectionName)
{
    return m_clientService->removeCollection(collectionName).toBool();
}

bool QEjdbRpcWorker::save(const QString &collectionName, QBsonObject &bson)
{
    ReturnValue retValue = m_clientService->save(collectionName, bson);

    if (!retValue.isError()) {

        bson.insert("_id", QBsonOid(retValue.toString()));
        return true;
    }
    return false;
}

QBsonObject QEjdbRpcWorker::load(const QString &collectionName, const QString &oid)
{
    ReturnValue retValue = m_clientService->load(collectionName, oid);
    return retValue.value<QBsonObject>();
}

bool QEjdbRpcWorker::remove(const QString &collectionName, const QString &oid)
{
    ReturnValue retValue = m_clientService->remove(collectionName, oid);
    return retValue.toBool();
}

QEjdbResult QEjdbRpcWorker::query(const QString &collectionName, const QBsonObject &query)
{
    ReturnValue retValue = m_clientService->query(collectionName, query);
    return retValue.value<QEjdbResult>();
}

void QEjdbRpcWorker::checkReturnValue(const QtRpc::ReturnValue &val)
{
    if (val.isError()) {
        qDebug() << val.errString();
    }
}


QEjdbRpcClientService::QEjdbRpcClientService(QObject *parent)
{

}
