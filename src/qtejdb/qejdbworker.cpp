#include "qejdbworker.h"
#include "qejdbdatabase.h"

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
