#include "qbsonoid.h"
#include <QSharedData>
#include "qatomic.h"
#include "bson.h"
/**
 * @brief The QBsonOidData class bson id wrapper.
 */
class QBsonOidData {
public:


    QBsonOidData()
    {
        ref = 1;
        // generate a bson id.
        bson_oid_t oid;
        bson_oid_gen(&oid);
        char strOid[25];
        bson_oid_to_string(&oid, strOid);
        id = QString(strOid);
    }

    QBsonOidData(const QString& id):id(id) { ref = 1;}
    QAtomicInt ref;
    QString id;
};

/**
 * @brief QBsonOidData construct a QBsonOid and generate a id.
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
QString QBsonOid::value() const
{
    return data->id;
}

/**
 * @brief QBsonOid::isValid true if the id is valid otherwise false.
 *
 * @return true if the id is valid otherwise false.
 */
bool QBsonOid::isValid() const
{
    return data->id.length() == 25;
}
