#ifndef QEJDBCOLLECTION_H
#define QEJDBCOLLECTION_H

#include <QString>
#include <QJsonObject>
#include "ejdb.h"

class QEjdbCollection
{

public:

    QEjdbCollection(EJDB* db, EJCOLL* col, QString m_collectionName);

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
    QString collectionName() const { return m_collectionName;}

private:
    /**
     * @brief m_db reference to database
     */
    EJDB* m_db;

    /**
     * @brief m_col reference to collection structure.
     */
    EJCOLL* m_col;

    /**
     * @brief m_collectionName columnname.
     */
    QString m_collectionName;

    /**
     * @brief convert2Bson convert the given jsonobject to the bson repräsentation
     * @param obj
     * @return
     */
    static bson convert2Bson(QJsonObject obj);

    /**
     * @brief convert2Bson2 recursive method to convert json 2 bson.
     * @param obj
     * @param bsrec
     */
    static void convert2Bson2(QJsonObject obj, bson &bsrec);




};

#endif // QEJDBCOLLECTION_H
