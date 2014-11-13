#include "websocketlistener.h"
#include "qejdbdatabaseservice.h"
#include <Server>
#include <ServerProtocolListenerTcp>
#include <QDebug>
#include <QThread>


WebSocketListener::WebSocketListener(qint16 port, ServerConfiguration *config) :
    ServerListener(config), m_config(config)
{

    connect(this, &ServerListener::start, this, &WebSocketListener::startServer, Qt::QueuedConnection);

    //m_workerThread = new QThread(this);

}

void WebSocketListener::startServer()
{
    //Create a server object with default threading options
    QtRpc::Server srv;

    qDebug() << "start server " << m_config->databaseName();

    //Create a TCP listener object
    QtRpc::ServerProtocolListenerTcp tcp(&srv);

    //Listen on port 10123 on all network interfaces
    if(!tcp.listen(QHostAddress::Any, 10123))
    {
        //This function returns false if the port is busy
        qCritical() << "Failed to listen on port 10123!";

    }

    //srv.moveToThread(m_workerThread);
    //m_workerThread->start();

    //Register a service. The template argument is the ServiceProxy class to use
    //The string argument is the name used to connect to the service
    //A new instance of the BasicService class is created each time a client connects
    srv.registerService<QEjdbDatabaseService>("QEjdbDatabase");


}
