// license
#include "qbsonobject.h"
#include <QSharedData>
#include "bson.h"
#include <QVariantMap>

class QBsonObjectData : public QSharedData {

public:

    QVariantMap data;

    bson convert2Bson(QBsonObject obj);
    void convert2Bson2(const char* attr, QJsonValue value, bson &bsrec);
    void convert2Query(bson* bson, QEjdbCondition condition);
    void convert2BsonEntry(bson *bson, const char* attr, QVariant value);

    QJsonObject convert2QJson(bson* bson);
    void convert2QJson2(QJsonObject &obj, bson_iterator *it);
    void convert2QJson2(QJsonArray &obj, bson_iterator *it);
    QJsonValue convert2QJsonValue(bson_type bt, bson_iterator *it);

};

/**
 * @brief QBsonObject::QBsonObject construct a new Empty QBsonObject
 */
QBsonObject::QBsonObject() : data(new QBsonObjectData)
{

}

/**
 * @brief QBsonObject::QBsonObject construct a QBsonObject from stream.
 * @param bson
 */
QBsonObject::QBsonObject(QByteArray bson) : data(new QBsonObjectData) {

}

/**
 * @brief QBsonObject::QBsonObject create a shared instance.
 * @param rhs
 */
QBsonObject::QBsonObject(const QBsonObject &rhs) : data(rhs.data)
{
}

/**
 * @brief QBsonObject::operator = create a shared instance.
 * @param rhs
 * @return
 */
QBsonObject &QBsonObject::operator=(const QBsonObject &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

QBsonObject::~QBsonObject()
{
}
