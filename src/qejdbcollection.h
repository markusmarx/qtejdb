#ifndef QEJDBCOLLECTION_H
#define QEJDBCOLLECTION_H

#include <QString>
#include <QJsonObject>
#include "qatomic.h"

struct EJDB;
struct EJCOLL;
class QEjdbCollectionPrivate;
class QEjdbDatabasePrivate;
class QEjdbCollection
{
    friend class QEjdbDatabasePrivate;

public:

    /**
     * @brief save save a jsonobject. if a jsonobject has a _id property then the document ist updated.
     * @param obj
     * @return if successful true otherwise false.
     */
    bool save(QJsonObject &obj);

    /**
     * @brief load load a json by object id.
     * @param oid object id
     * @return jsonobject.
     */
    QJsonObject load(QString oid);

    /**
     * @brief collectionName return the name of the collection
     * @return collectionName
     */
    QString collectionName() const;

    QEjdbCollection(const QEjdbCollection &other);

    QEjdbCollection &operator =(const QEjdbCollection &other);

    bool removeCollection();

    ~QEjdbCollection();

protected:

    QEjdbCollection(EJDB* db, EJCOLL* col, QString m_collectionName);

private:

    QEjdbCollectionPrivate *d;


};

#endif // QEJDBCOLLECTION_H
