#include <QCoreApplication>
#include <QTest>
#include "tst_database.h"
#include "tst_collection.h"
#include "tst_performance.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTest::qExec(new Tst_Database, argc, argv);
    QTest::qExec(new Tst_Collection, argc, argv);
    QTest::qExec(new Tst_Performance, argc, argv);

    //return a.exec();
}
