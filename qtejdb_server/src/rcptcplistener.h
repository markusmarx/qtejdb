#ifndef WEBSOCKETLISTENER_H
#define WEBSOCKETLISTENER_H

#include <QObject>
#include "serverlistener.h"
#include "serverconfiguration.h"
#include <ServerProtocolListenerTcp>
#include <Server>

QT_FORWARD_DECLARE_CLASS(QThread)

class RcpTcpListener : public ServerListener
{
    Q_OBJECT
public:
    explicit RcpTcpListener(qint16 port, ServerConfiguration *config);

signals:

public slots:
    void startServer();
private:
    /**
     * @brief m_config server config.
     */
    ServerConfiguration *m_config;

    /**
     * @brief m_listener tcp listener.
     */
    QtRpc::ServerProtocolListenerTcp *m_listener;

    /**
     * @brief m_server server.
     */
    QtRpc::Server *m_server;

    /**
     * @brief m_port
     */
    int16 m_port;

};

#endif // WEBSOCKETLISTENER_H
