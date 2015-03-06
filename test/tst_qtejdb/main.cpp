#include <QCoreApplication>
#include <QTest>
#include "tst_qejdbdatabase.h"
#include "tst_qejdbcollection.h"
#include "tst_qejdbperformance.h"
#include "tst_qbsonmodel.h"
#include <QObject>
#include <QScopedPointer>
#include <QDebug>

int main(int argc, char *argv[])
{
    qDebug() << argv[0];
    QCoreApplication a(argc, argv);

    try {
        QScopedPointer<Tst_QEjdbDatabase> t1(new Tst_QEjdbDatabase());

        QScopedPointer<Tst_QEjdbCollection> t2 (
                    new Tst_QEjdbCollection("file:test_db")
                    );

        QScopedPointer<Tst_QEjdbPerformance> t3 (
                    new Tst_QEjdbPerformance("file:test_db")
                    );

        QScopedPointer<Tst_QEjdbCollection> t4 (
                    new Tst_QEjdbCollection("socket:qtejdbtest")
                    );

        QScopedPointer<Tst_QEjdbPerformance> t5 (
                    new Tst_QEjdbPerformance("socket:qtejdbtest")
                    );
        QScopedPointer<Tst_QBsonModel> t6 (
                    new Tst_QBsonModel()
                    );


        /*QTest::qExec(t1.data(), argc, argv);
        QTest::qExec(t2.data(), argc, argv);
        QTest::qExec(t3.data(), argc, argv);
        QTest::qExec(t4.data(), argc, argv);
        QTest::qExec(t5.data(), argc, argv);*/
        QTest::qExec(t6.data(), argc, argv);


    } catch(QEjdbException &ex) {
        qDebug() << ex;
    }
}
