#ifndef QBSONFROMBSONREC_H
#define QBSONFROMBSONREC_H

#include "qbsonvalue.h"
#include "qbsonobject.h"
#include "qbsonobject_p.h"
#include "bson.h"

QBsonValue Q_DECL_EXPORT convertQBsonValueFromBsonRec(bson_type bt, bson_iterator *it);
void Q_DECL_EXPORT convertBsonValueFromBsonRec(QBsonArray &obj, bson_iterator *it);
void Q_DECL_EXPORT convertBsonValueFromBsonRec(QBsonObjectData *values, bson_iterator *it);
void Q_DECL_EXPORT convertBsonObjectFromBsonRec(bson *bson, QBsonObjectData *obj);
void Q_DECL_EXPORT convertBsonObjectFromBsonRec(bson *bson, QBsonObject &obj);
void Q_DECL_EXPORT convertBsonValueFromBsonRec(QBsonObject &obj, bson_iterator *it);

#endif // QBSONFROMBSONREC_H
