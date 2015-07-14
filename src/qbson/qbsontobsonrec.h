#ifndef QBSONTOBSONREC_H
#define QBSONTOBSONREC_H

#include "bson.h"
#include "qbsonobject.h"
#include "qbsonvalue.h"


void appendQBsonValueToBsonRec(bson *bson, const char* attr, const QBsonValue &value);
bson convertQBsonToBsonRec(const QBsonObject &obj, bson* bs);
bson convertQBsonToBsonRec(const QBsonObjectData *obj, bson* bs);
bson convertQBsonToBsonRec(const QBsonValueHash values, QBsonOid oid, bson* bs);

#endif // QBSONTOBSONREC_H
