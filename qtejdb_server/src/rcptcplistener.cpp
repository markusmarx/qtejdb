#include "rcptcplistener.h"
#include "qejdbdatabaseservice.h"
#include <Server>
#include <QDebug>
#include <QThread>
#include <QDir>

RcpSocketListener::RcpSocketListener(ServerConfiguration *config) :
    ServerListener(config), m_config(config)
{

    connect(this, &ServerListener::start, this, &RcpSocketListener::startServer, Qt::QueuedConnection);

}

void RcpSocketListener::startServer()
{
    //Create a server object with default threading options
    m_server = new QtRpc::Server;

    //qDebug() << "start server " << m_config->databaseName() << " on port " << m_port;
    qDebug() << "start server " << m_config->databaseName() << " with name " << m_config->databaseName();
    //Create a TCP listener object
    //m_listener = new ServerProtocolListenerTcp(m_server);

    m_listener = new ServerProtocolListenerSocket(m_server);

    //if(!m_listener->listen(QHostAddress::Any, m_port))
    if(!m_listener->listen(QDir().temp().absoluteFilePath(m_config->databaseName()),
                           ServerProtocolListenerSocket::Owner).toBool())
    {
        //This function returns false if the port is busy
        qCritical() << "Failed to listen on port " << m_port <<  "!";

    } else {
        m_server->registerService<QEjdbDatabaseService>(m_config->databaseName());
    }
}
