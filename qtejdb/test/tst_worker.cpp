#include "tst_worker.h"
#include "qejdbworker.h"
#include <QTest>

Tst_Worker::Tst_Worker(QObject *parent) :
    QObject(parent)
{
}

void Tst_Worker::tst_fileWorker()
{

    QEjdbWorker *worker = QEjdbWorker::createFromUrl(QUrl("file:test.db"), 0);

    QEjdbFileWorker *fileWorker = dynamic_cast<QEjdbFileWorker*>(worker);

    QVERIFY(fileWorker!=0);

}
