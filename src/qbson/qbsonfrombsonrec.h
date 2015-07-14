#ifndef QBSONFROMBSONREC_H
#define QBSONFROMBSONREC_H

#include "qbsonvalue.h"
#include "qbsonobject.h"
#include "qbsonobject_p.h"
#include "bson.h"

QBsonValue convertQBsonValueFromBsonRec(bson_type bt, bson_iterator *it);
void convertBsonValueFromBsonRec(QBsonArray &obj, bson_iterator *it);
void convertBsonValueFromBsonRec(QBsonObjectData *values, bson_iterator *it);
void convertBsonObjectFromBsonRec(bson *bson, QBsonObjectData *obj);
void convertBsonObjectFromBsonRec(bson *bson, QBsonObject &obj);
void convertBsonValueFromBsonRec(QBsonObject &obj, bson_iterator *it);

#endif // QBSONFROMBSONREC_H
