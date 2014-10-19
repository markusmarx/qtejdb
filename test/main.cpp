#include <QCoreApplication>
#include <QTest>
#include "tst_database.h"
#include "tst_collection.h"
#include "tst_performance.h"
#include "tst_qbson.h"
#include <QObject>
#include <QScopedPointer>

int main(int argc, char *argv[])
{

    QTest::qExec(new Tst_QBson, argc, argv);
    QTest::qExec(new Tst_Database, argc, argv);
    QTest::qExec(new Tst_Collection(), argc, argv);
    QTest::qExec(new Tst_Performance, argc, argv);


}
