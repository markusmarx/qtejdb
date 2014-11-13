#include "qejdbdatabaseservice.h"

QTRPC_REGISTER_METATYPE(QBsonObject)

QEjdbDatabaseService::QEjdbDatabaseService(QObject *parent) :
    ClientProxy(parent)
{
}
