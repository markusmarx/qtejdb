#ifndef WEBSOCKETLISTENER_H
#define WEBSOCKETLISTENER_H

#include <QObject>
#include "serverlistener.h"
#include "serverconfiguration.h"

QT_FORWARD_DECLARE_CLASS(QThread)

class WebSocketListener : public ServerListener
{
    Q_OBJECT
public:
    explicit WebSocketListener(qint16 port, ServerConfiguration *config);

signals:

public slots:
    void startServer();
private:
    ServerConfiguration *m_config;
    QThread *m_workerThread;


};

#endif // WEBSOCKETLISTENER_H
