#ifndef TST_CONNECTION_H
#define TST_CONNECTION_H

#include <QObject>
#include "qbson/qbsonobject.h"

class Tst_Connection : public QObject
{
    Q_OBJECT
public:
    explicit Tst_Connection(QObject *parent = 0);

signals:

private slots:

    void tst_simple();

private:
    QBsonObject createTestBson();

};

#endif // TST_CONNECTION_H
