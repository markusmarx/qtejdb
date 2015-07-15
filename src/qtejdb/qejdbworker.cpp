#include "qejdbworker.h"
#include "bson.h"
#include "qbson/qbsonobject_p.h"
#include "qejdbdatabase.h"
#include <QDebug>

QEjdbWorker::QEjdbWorker()
{
}

QEjdbWorker *QEjdbWorker::createFromUrl(const QUrl &url, int mode)
{
    QEjdbWorker *worker = 0;
    if (url.isLocalFile()) {
    	if (mode == 0) {
            mode = QEJDB::CREATE | QEJDB::WRITE
                   | QEJDB::SYNC | QEJDB::LOCK_NB;
    	}
        worker = new QEjdbFileWorker(url, mode);
    }

    return worker;
}
