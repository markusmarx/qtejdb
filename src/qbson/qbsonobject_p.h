#ifndef QBSONOBJECT_P_H
#define QBSONOBJECT_P_H
#include "qatomic.h"
#define TCNUMBUFSIZ 32

QT_FORWARD_DECLARE_CLASS(QBsonObject)

class QBsonObjectData  {

public:
    QBsonObjectData()

    {
        ref = 1;
    }

    QBsonValueHash values;
    QByteArray toBinary() const;

    static void fromBinary(const QByteArray& binary, QBsonObject& obj);
    static void fromBinary(const QByteArray& binary, QBsonObjectData *obj);
    static bson convert2Bson(const QBsonObjectData &obj, bson* bson = 0);
    static void convert2Bson2(const char* attr, QBsonValue value, bson &bsrec);
    static void convert2BsonEntry(bson *bson, const char* attr, QBsonValue &value);

    static void convert2QBson(bson* bson, QBsonObject &obj);
    static void convert2QBson(bson* bson, QBsonObjectData *obj);
    static void convert2QBson2(QBsonObjectData *obj, bson_iterator *it);
    static void convert2QBson2(QBsonObject &obj, bson_iterator *it);
    static void convert2QBson2(QBsonArray &obj, bson_iterator *it);
    static QBsonValue convert2QBsonValue(bson_type bt, bson_iterator *it);

    void deleteCache();
    void insert(const QString &name, const QBsonValue &value);

    QAtomicInt ref;

    friend class QBsonObject;


};


#endif // QBSONOBJECT_P_H
