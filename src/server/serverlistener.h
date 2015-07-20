#ifndef SERVERLISTENER_H
#define SERVERLISTENER_H

#include <QObject>

class ServerListener : public QObject
{
    Q_OBJECT
public:
    explicit ServerListener(QObject *parent = 0);

signals:
    void start();
public slots:



};

#endif // SERVERLISTENER_H
