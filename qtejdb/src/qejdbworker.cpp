#include "qejdbworker.h"
#include "qejdbrpcworker.h"
#include "bson.h"
#include "bson/qbsonobject.h"
#include "bson/qbsonobject_p.h"
#include <QDebug>

QEjdbWorker::QEjdbWorker()
{
}

QEjdbWorker *QEjdbWorker::createFromUrl(const QUrl &url, int mode)
{
    QEjdbWorker *worker;
    if (url.isLocalFile()) {
        worker = new QEjdbFileWorker(url, mode);
    } else if (url.scheme() == QStringLiteral("rcp")) {
        worker = new QEjdbRpcWorker(url, mode);
    }

    return worker;
}
