#ifndef SERVERCONFIGURATION_H
#define SERVERCONFIGURATION_H

#include <QObject>
#include <QDir>
#include "qejdbdatabase.h"

QT_FORWARD_DECLARE_CLASS(ServerListener)

/**
 * @brief The ServerConfiguration class manage serverconfiguration.
 *
 */
class ServerConfiguration : public QObject
{
    Q_OBJECT
public:
    explicit ServerConfiguration(QObject *parent = 0);

    /**
     * @brief setDatabasePath set the path where the db is stored.
     *
     * @param path store path
     */
    void setDatabasePath(const QString &path);

    /**
     * @brief setDatabaseName set database name. Name is used to stored db in hash and the
     * database file name.
     *
     * @param databaseName name of the database.
     */
    void setDatabaseName(const QString &databaseName);

    /**
     *
     * @brief addServerListener
     * @param serverListener
     */
    void addServerListener(ServerListener *serverListener);

signals:

    void shutdown();

public slots:

    /**
     * @brief run start the serverconfiguration
     */
    void run();

private:

    /**
     * @brief m_database reference to local database.
     */
    QEjdbDatabase m_database;

    /**
     * @brief m_databasePath path to database directory.
     */
    QString m_databasePath;

    /**
     * @brief m_databaseName database name
     */
    QString m_databaseName;

    /**
     * @brief m_listener list of serverlistener.
     */
    QList<ServerListener*> m_listener;

};

#endif // SERVERCONFIGURATION_H
