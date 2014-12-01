#ifndef QEJDBCOLLECTION_H
#define QEJDBCOLLECTION_H

#include <QString>
#include <QHash>
#include "qatomic.h"

struct EJDB;
struct EJCOLL;
class QEjdbCollectionPrivate;
class QEjdbDatabasePrivate;
class QBsonObject;

class QEjdbCollection
{
    friend class QEjdbDatabasePrivate;
    friend class QHash<QString, QEjdbCollection>;
public:

    /**
     * @brief save save a jsonobject. if a jsonobject has a _id property then the document ist updated.
     * @param obj
     * @return if successful true otherwise false.
     */
    bool save(QBsonObject &obj);

    /**
     * @brief load load a json by object id.
     * @param oid object id
     * @return jsonobject.
     */
    QBsonObject load(QString oid);

    bool remove(QBsonObject &obj);

    /**
     * @brief collectionName return the name of the collection
     * @return collectionName
     */
    QString collectionName() const;



    QEjdbCollection(const QEjdbCollection &other);

    QEjdbCollection &operator =(const QEjdbCollection &other);

    bool removeCollection();

    QList<QBsonObject> query(const QBsonObject& query);

    /**
     * @brief isValid Returns true if the EjdbCollection is a valid collection, else false.
     *
     * @return Returns true if the EjdbCollection is a valid collection, else false.
     */
    bool isValid() const;

    ~QEjdbCollection();

protected:

    QEjdbCollection(const QString& collectionName = QString());
    QEjdbCollection(EJDB* db, EJCOLL* col, QString m_collectionName);

private:

    QEjdbCollectionPrivate *d;


};

#endif // QEJDBCOLLECTION_H
