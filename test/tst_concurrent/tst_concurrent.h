#ifndef TST_CONNECTION_H
#define TST_CONNECTION_H

#include <QObject>
#include "qbson/qbsonobject.h"

class Tst_Concurrent : public QObject
{
    Q_OBJECT
public:
    explicit Tst_Concurrent(QObject *parent = 0);

signals:

private slots:

    void tst_simple();
    void initTestCase();
    void cleanupTestCase();
private:
    QBsonObject createTestBson();

};

#endif // TST_CONNECTION_H
