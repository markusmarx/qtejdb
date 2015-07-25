/*
 * Copyright (C) 2014-2015 Markus Marx <markus.marx@marxenter.de>
 *
 * This file is part of the QtEjdb.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 */
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
class Q_DECL_EXPORT QBsonOid
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
    uint hash() const;
    bool isValid() const;
    bool operator ==(const QBsonOid& id) const;
    operator QString();

    static QBsonOid generate();

private:
    QBsonOidData *data;
};

Q_DECLARE_METATYPE(QBsonOid)
#endif // QBSONOID_H
