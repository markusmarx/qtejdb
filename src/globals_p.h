#ifndef GLOBALS_P_H
#define GLOBALS_P_H
#include "ejdb.h"
#include <QJsonObject>
#include <QJsonValue>
#include "qejdbcondition.h"

#define TCNUMBUFSIZ    32

namespace QEJDB {

    bson convert2Bson(QJsonObject obj);
    void convert2Bson2(const char* attr, QJsonValue value, bson &bsrec);
    void convert2Query(bson* bson, QEjdbCondition condition);
    void convert2BsonEntry(bson *bson, const char* attr, QVariant value);

    QJsonObject convert2QJson(bson* bson);
    void convert2QJson2(QJsonObject &obj, bson_iterator *it);
    void convert2QJson2(QJsonArray &obj, bson_iterator *it);
    QJsonValue convert2QJsonValue(bson_type bt, bson_iterator *it);

}


#endif // GLOBALS_P_H
