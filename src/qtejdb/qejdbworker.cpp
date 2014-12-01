#include "qejdbworker.h"
#include "qejdbrpcworker.h"
#include "bson.h"
#include "qbsonobject_p.h"
#include "qejdbdatabase.h"
#include <QDebug>

QEjdbWorker::QEjdbWorker()
{
}

QEjdbWorker *QEjdbWorker::createFromUrl(const QUrl &url, int mode)
{
    QEjdbWorker *worker;
    if (url.isLocalFile()) {
    	if (mode == 0) {
    		mode = QEJDB::CREATE | QEJDB::WRITE
                   | QEJDB::LOCK_NB | QEJDB::TRUNCATE;
    	}
        worker = new QEjdbFileWorker(url, mode);
    } else if (url.scheme() == QStringLiteral("tcp")
               || url.scheme() == QStringLiteral("socket")) {
        worker = new QEjdbRpcWorker(url, mode);
    }

    return worker;
}
