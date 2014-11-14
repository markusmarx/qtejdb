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

    qDebug() << "start server " << m_config->databaseName();

    //Create a TCP listener object
    m_listener = new ServerProtocolListenerTcp(m_server);

    //Listen on port 10123 on all network interfaces
    if(!m_listener->listen(QHostAddress::Any, 10123))
    {
        //This function returns false if the port is busy
        qCritical() << "Failed to listen on port 10123!";

    }

    //srv.moveToThread(m_workerThread);
    //m_workerThread->start();

    //Register a service. The template argument is the ServiceProxy class to use
    //The string argument is the name used to connect to the service
    //A new instance of the BasicService class is created each time a client connects
    m_server->registerService<QEjdbDatabaseService>("QEjdbDatabase");


}
