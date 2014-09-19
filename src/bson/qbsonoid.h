#ifndef QBSONOID_H
#define QBSONOID_H
#include <QString>
#include <QObject>

class QBsonOidData;

class QBsonOid
{
public:
    QBsonOid();
    QBsonOid(const QString& id);
    QBsonOid(const QLatin1String& id);
    QBsonOid(const char* id);
    QBsonOid(const QString id);
    QBsonOid(const QBsonOid &);

    QBsonOid &operator=(const QBsonOid &);
    ~QBsonOid();

    QString value() const;
private:
    QBsonOidData *data;
};

Q_DECLARE_METATYPE(QBsonOid)
#endif // QBSONOID_H
