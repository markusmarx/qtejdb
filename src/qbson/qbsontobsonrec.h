#ifndef QBSONTOBSONREC_H
#define QBSONTOBSONREC_H

#include "bson.h"
#include "qbsonobject.h"
#include "qbsonvalue.h"


void Q_DECL_EXPORT appendQBsonValueToBsonRec(bson *bson, const char* attr, const QBsonValue &value);
bson Q_DECL_EXPORT convertQBsonToBsonRec(const QBsonObject &obj, bson* bs);
bson Q_DECL_EXPORT convertQBsonToBsonRec(const QBsonObjectData *obj, bson* bs);
bson Q_DECL_EXPORT convertQBsonToBsonRec(const QBsonValueHash values, QBsonOid oid, bson* bs);

#endif // QBSONTOBSONREC_H
