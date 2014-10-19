#ifndef QBSONVALUE_H
#define QBSONVALUE_H

#include <QDateTime>
#include <QLatin1String>

class QBsonValueData;
class QBsonObject;
class QBsonArray;
class QBsonOid;

class QBsonValue
{
public:

    /**
     * @brief The QBsonValueType bsonvalue types.
     */
    enum QBsonValueType {
        Double, Integer, Long, Object, Array, Id, DateTime, Timestamp, Binary, Empty,
        Bool, String
    };

    QBsonValue();
    QBsonValue(int value);
    QBsonValue(const QBsonObject &value);
    QBsonValue(const QString &value);
    QBsonValue(const QLatin1String &value);
    QBsonValue(const char *value);
    QBsonValue(double value);
    QBsonValue(long value);
    QBsonValue(const QDateTime &value);
    QBsonValue(const QByteArray &value);
    QBsonValue(const QBsonArray &value);
    QBsonValue(bool value);
    QBsonValue(const QBsonOid &value);


    QBsonValue(const QBsonValue &);
    QBsonValue &operator=(const QBsonValue &);
    ~QBsonValue();

    QBsonValue::QBsonValueType type() const;
    QString toString() const;
    int toInt() const;
    double toDouble() const;
    long toLong() const;
    QByteArray toBinary() const;
    QBsonObject toObject() const;
    QDateTime toDateTime() const;
    QBsonArray toArray() const;
    bool toBool() const;
    QBsonOid toId() const;
    bool operator ==(const QBsonValue value) const;
    bool isObject() const;
    bool isArray() const;

private:
    QBsonValueData *data;

};

#endif // QBSONVALUE_H
