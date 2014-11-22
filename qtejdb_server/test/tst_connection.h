#ifndef TST_CONNECTION_H
#define TST_CONNECTION_H

#include <QObject>

class Tst_Connection : public QObject
{
    Q_OBJECT
public:
    explicit Tst_Connection(QObject *parent = 0);

signals:

private slots:

    void tst_simple();

};

#endif // TST_CONNECTION_H
