#include "qbsonoid.h"
#include <QSharedData>
#include "qatomic.h"
#include "bson.h"
#include <QDebug>

/**
 * @brief The QBsonOidData class bson id wrapper.
 */
class QBsonOidData {
public:

    /**
     * @brief QBsonOidData generate a valid bson id. Bson Id is a
     * 12-byte array.
     * @link http://docs.mongodb.org/manual/reference/object-id/
     * @param generate if true a bson id is generated.
     */
    QBsonOidData()
    {
        ref = 1;
        //id = "";
    }

    QBsonOidData(const QString& id):id(id) { ref = 1;}
    QAtomicInt ref;
    QString id;
};

/**
 * @brief QBsonOidData construct a empty QBsonOid.
 */
QBsonOid::QBsonOid():
    data(new QBsonOidData)
{

}

/**
 * @brief QBsonOidData construct a QBsonOidData from QString
 *
 * @param id QString with 25 characters
 */
QBsonOid::QBsonOid(const QString& id):
    data(new QBsonOidData(id))
{
}

/**
 * @brief QBsonOid::QBsonOid construct a QBsonOid from QLatin1String.
 *
 * @param id QLatrin1String with 25 characters.
 */
QBsonOid::QBsonOid(const QLatin1String &id):
    data(new QBsonOidData(id))
{

}

/**
 * @brief QBsonOid::QBsonOid construct a QBsonOid from const char*.
 *
 * @param id char array with 25 characters.
 */
QBsonOid::QBsonOid(const char *id):
    data(new QBsonOidData(QString(id)))
{

}

/**
 * @brief QBsonOid::QBsonOid copy constructor
 * @param rhs
 */
QBsonOid::QBsonOid(const QBsonOid &rhs) : data(rhs.data)
{
    data->ref.ref();
}

/**
 * @brief QBsonOid::operator = copy a shared instance.
 * @param rhs
 * @return
 */
QBsonOid &QBsonOid::operator=(const QBsonOid &rhs)
{
    qAtomicAssign<QBsonOidData>(data, rhs.data);
    return *this;
}

/**
 * @brief QBsonOid::~QBsonOid destruct object and delete shared instance when
 * not referenced.
 */
QBsonOid::~QBsonOid()
{
    if (!data->ref.deref())
        delete data;
}

/**
 * @brief QBsonOid::value get id as QString
 *
 * @return id as QString
 */
QString QBsonOid::toString() const
{
    return data->id;
}

/**
 * @brief QBsonOid::hash Return the hash from id string. @see qHash()
 *
 * @return hash value from string
 */
uint QBsonOid::hash() const
{
    return qHash(data->id);
}

/**
 * @brief QBsonOid::isValid true if the id is valid otherwise false.
 *
 * @return true if the id is valid otherwise false.
 */
bool QBsonOid::isValid() const
{
    return data->id.length() == 24;
}

bool QBsonOid::operator ==(const QBsonOid &id) const
{
    return data->id == id.toString();
}

/**
 * @brief QBsonOid::generate generate a new QBsonOid.
 *
 * @return a new QBsonOid
 */
QBsonOid QBsonOid::generate()
{
    // generate a bson id.
    bson_oid_t oid;
    bson_oid_gen(&oid);
    char strOid[25];
    bson_oid_to_string(&oid, strOid);
    return QBsonOid(QString(strOid));
}

QBsonOid::operator QString()
{
    return data->id;
}


QDebug operator<<(QDebug dbg, const QBsonOid &oid)
{
    dbg << oid.toString();
    return dbg.space();
}
