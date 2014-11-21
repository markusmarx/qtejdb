#include "qejdbrpcworker.h"

QEjdbRpcWorker::QEjdbRpcWorker(const QUrl &url, int mode)
{
}

void QEjdbRpcWorker::open()
{

}

bool QEjdbRpcWorker::isOpen()
{

}

bool QEjdbRpcWorker::close()
{

}

bool QEjdbRpcWorker::containsCollection(const QString &collectionName)
{

}

bool QEjdbRpcWorker::createCollection(const QString &collectionName)
{

}

bool QEjdbRpcWorker::removeCollection(const QString &collectionName)
{

}

bool QEjdbRpcWorker::save(const QString &collectionName, QBsonObject &bson)
{

}

QBsonObject QEjdbRpcWorker::load(const QString &collectionName, const QString &oid)
{

}

bool QEjdbRpcWorker::remove(const QString &collectionName, const QString &oid)
{

}

QList<QBsonObject> QEjdbRpcWorker::query(const QString &collectionName, const QBsonObject &query)
{

}
