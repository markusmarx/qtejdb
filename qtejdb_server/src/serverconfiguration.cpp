#include "serverconfiguration.h"
#include "serverlistener.h"
#include <QDebug>

ServerConfiguration::ServerConfiguration(QObject *parent) :
    QObject(parent)
{
}

void ServerConfiguration::setDatabasePath(const QString &path)
{
    m_databasePath = path;
}

void ServerConfiguration::setDatabaseName(const QString &databaseName)
{
    m_databaseName = databaseName;
}

void ServerConfiguration::addServerListener(ServerListener *serverListener)
{
    m_listener.append(serverListener);
}

void ServerConfiguration::run()
{
    QString dbUrl = "file:"+m_databasePath+QDir::separator()+m_databaseName;
    qDebug() << "open database " << dbUrl;
    m_database = QEjdbDatabase::addDatabase("file:./test",
                               QEJDB::CREATE | QEJDB::WRITE
                               | QEJDB::LOCK_NB | QEJDB::TRUNCATE, m_databaseName);

    if (m_database.open()) {
        foreach (ServerListener* listener, m_listener) {
            listener->start();
        }
    } else {
        qWarning() << "database not opened.";
    }

    emit shutdown();
}
