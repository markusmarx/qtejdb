#include "rcptcplistener.h"
#include "qejdbdatabaseservice.h"
#include <Server>
#include <QDebug>
#include <QThread>


RcpTcpListener::RcpTcpListener(qint16 port, ServerConfiguration *config) :
    ServerListener(config), m_config(config), m_port(port)
{

    connect(this, &ServerListener::start, this, &RcpTcpListener::startServer, Qt::QueuedConnection);

}

void RcpTcpListener::startServer()
{
    //Create a server object with default threading options
    m_server = new QtRpc::Server;

    qDebug() << "start server " << m_config->databaseName() << " on port " << m_port;

    //Create a TCP listener object
    m_listener = new ServerProtocolListenerTcp(m_server);

    if(!m_listener->listen(QHostAddress::Any, m_port))
    {
        //This function returns false if the port is busy
        qCritical() << "Failed to listen on port 10123!";

    } else {
        m_server->registerService<QEjdbDatabaseService>("QEjdbDatabase");
    }
}
