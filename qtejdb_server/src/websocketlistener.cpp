#include "websocketlistener.h"
#include "echoserver.h"

WebSocketListener::WebSocketListener(qint16 port, QObject *parent) :
    ServerListener(parent)
{
    m_server = new EchoServer(port, this);
}
