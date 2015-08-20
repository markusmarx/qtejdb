//
// Created by markus on 09.08.15.
//

#ifndef QTEJDB_QEJDBDATABASE_P_H
#define QTEJDB_QEJDBDATABASE_P_H

#include <QHash>
#include "qejdbworker.h"
#include "qejdbdatabase.h"
#include <QReadWriteLock>
#include <QStringList>


class QEjdbDatabasePrivate
{
public:
    QAtomicInt ref;
    QEjdbWorker *m_worker;
    QEjdbDatabasePrivate(
            QEjdbDatabase *d, const QUrl &url, int mode, const QString &connectionName
    ):q(d), m_worker(QEjdbWorker::createFromUrl(url, mode))
            ,m_connectionName(connectionName)
    {
        ref = 1;

    }

    ~QEjdbDatabasePrivate()
    {
        delete m_worker;
    }


    void open();
    bool close();
    bool isOpen() const;

    bool createCollection(const QString &collectionName);
    bool removeCollection(const QString &collectionName);
    bool containsCollection(const QString &collectionName);

    QString connectionName() const {
        return m_connectionName;
    }

    static void removeDatabase(const QString &name);
    static void addDatabase(const QEjdbDatabase &db, const QString &name);
    static QEjdbDatabase database(const QString &name, bool open);

    QHash<QString, QSharedPointer<QEjdbCollectionPrivate > > m_collections;
private:

    /**
     * @brief q
     */
    QEjdbDatabase *q;

    /**
     * @brief m_dbPath contains the path to the db files.
     */
    QDir m_dbPath;

    /**
     * @brief m_dbName contains the db name.
     */
    QString m_dbName;

    /**
     * @brief m_mode database open mode
     */
    int m_mode;

    /**
     * @brief m_connectionName name of the connection stored in dictionary.
     */
    QString m_connectionName;

    inline void checkConnection() const
    {
        if (!isOpen()) {
            throw QEjdbException(QEjdbException::NOTCONNECTED, "database is not connected.");
        }
    }

};

#endif //QTEJDB_QEJDBDATABASE_P_H
