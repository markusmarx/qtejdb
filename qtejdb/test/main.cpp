#include <QCoreApplication>
#include <QTest>
#include "tst_database.h"
#include "tst_collection.h"
#include "tst_performance.h"
#include "tst_qbson.h"
#include "tst_worker.h"
#include <QObject>
#include <QScopedPointer>
#include <QDebug>

int main(int argc, char *argv[])
{
    try {
        QTest::qExec(new Tst_QBson, argc, argv);
        //QTest::qExec(new Tst_Database, argc, argv);
        //QTest::qExec(new Tst_Collection(), argc, argv);
        //QTest::qExec(new Tst_Performance, argc, argv);
        //QTest::qExec(new Tst_Worker, argc, argv);
    } catch(QEjdbException &ex) {
        qDebug() << ex;
    }
}
