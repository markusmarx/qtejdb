#ifndef QEJDBWORKER_H
#define QEJDBWORKER_H
#include <QtGlobal>
#include <QUrl>
#include <QHash>

#include "bson/qbsonobject.h"
#include "ejdb.h"

class QEjdbWorker
{
public:
    QEjdbWorker();
    virtual ~QEjdbWorker() {}

    virtual void open() = 0;
    virtual bool isOpen() = 0;
    virtual bool close() = 0;

    virtual bool containsCollection(const QString &collectionName) = 0;
    virtual bool createCollection(const QString &collectionName) = 0;
    virtual bool removeCollection(const QString &collectionName) = 0;

    virtual bool save(const QString &collectionName, QBsonObject &bson) = 0;
    virtual QBsonObject load(const QString &collectionName, const QString &oid) = 0;
    virtual bool remove(const QString &collectionName, const QString &oid) = 0;

    virtual QList<QBsonObject> query(const QString &collectionName, const QBsonObject &query) = 0;

    /**
     * @brief createFromUrl creates a worker instance from url.
     *
     * @param url
     * @param mode
     * @return
     */
    static QEjdbWorker *createFromUrl(const QUrl &url, int mode);

};

class QEjdbFileWorker: public QEjdbWorker
{
public:
    QEjdbFileWorker(const QUrl &url, int mode);
    ~QEjdbFileWorker() {}

    /**
     * @brief open open the database under the given file path.
     *
     * @throw QEjdbException if database could not opened.
     * @return void
     */
    virtual void open();
    virtual bool isOpen();
    virtual bool close();

    virtual bool containsCollection(const QString &collectionName);
    virtual bool createCollection(const QString &collectionName);
    virtual bool removeCollection(const QString &collectionName);

    virtual bool save(const QString &collectionName, QBsonObject &bson);
    virtual QBsonObject load(const QString &collectionName, const QString &oid);
    virtual bool remove(const QString &collectionName, const QString &oid);

    virtual QList<QBsonObject> query(const QString &collectionName, const QBsonObject &query);
private:
    QString m_path;
    int m_mode;

    /**
     * @brief m_db pointer to ejdb.
     */
    EJDB* m_db;
    QHash<QString, EJCOLL*> m_collectionDict;

    EJCOLL *getCollection(const QString &collectionName);

};



#endif // QEJDBWORKER_H
