#ifndef QEJDBDATABASESERVICE_H
#define QEJDBDATABASESERVICE_H

#include <ClientProxy>
#include "bson/qbsonobject.h"

using namespace QtRpc;


class QEjdbDatabaseService : public ClientProxy
{
    Q_OBJECT

    QTRPC_CLIENTPROXY(QEjdbDatabaseService)
public:
    explicit QEjdbDatabaseService(QObject *parent = 0);

signals:

    ReturnValue save(QString collection, QBsonObject obj);
    ReturnValue createCollection(QString collectionName);
    ReturnValue removeCollection(QString collectionName);

public slots:

};

#endif // QEJDBDATABASESERVICE_H
