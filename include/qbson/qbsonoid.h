#ifndef QBSONOID_H
#define QBSONOID_H
#include <QString>
#include <QObject>

class QBsonOidData;

/**
 * @brief The QBsonOid class wrapper to store a bson id value. Bson Id is a
 * 12-byte array.
 * @see http://docs.mongodb.org/manual/reference/object-id/
 */
class QBsonOid
{
public:
    QBsonOid();
    QBsonOid(const QString& id);
    QBsonOid(const QLatin1String& id);
    QBsonOid(const char* id);
    QBsonOid(const QBsonOid &);

    QBsonOid &operator=(const QBsonOid &);
    ~QBsonOid();

    QString toString() const;
    bool isValid() const;
    bool operator ==(const QBsonOid& id) const;
    operator QString();

private:
    QBsonOidData *data;
};

Q_DECLARE_METATYPE(QBsonOid)
#endif // QBSONOID_H
