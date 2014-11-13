#ifndef TST_CONNECTION_H
#define TST_CONNECTION_H

#include <QObject>

class tst_connection : public QObject
{
    Q_OBJECT
public:
    explicit tst_connection(QObject *parent = 0);

signals:

public slots:

    void tst_simple();

};

#endif // TST_CONNECTION_H
