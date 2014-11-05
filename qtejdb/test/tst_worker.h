#ifndef TST_WORKER_H
#define TST_WORKER_H

#include <QObject>

class Tst_Worker : public QObject
{
    Q_OBJECT
public:
    explicit Tst_Worker(QObject *parent = 0);

signals:

private slots:
    //void initTestCase();
    void tst_fileWorker();
};

#endif // TST_WORKER_H
