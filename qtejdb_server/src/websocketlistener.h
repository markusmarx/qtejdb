#ifndef WEBSOCKETLISTENER_H
#define WEBSOCKETLISTENER_H

#include <QObject>
#include "serverlistener.h"

QT_FORWARD_DECLARE_CLASS(EchoServer)

class WebSocketListener : public ServerListener
{
    Q_OBJECT
public:
    explicit WebSocketListener(qint16 port, QObject *parent = 0);

signals:

public slots:

private:
    EchoServer *m_server;

};

#endif // WEBSOCKETLISTENER_H
