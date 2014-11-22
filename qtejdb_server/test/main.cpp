#include <QCoreApplication>
#include "tst_connection.h"
#include <QDebug>
#include <QTest>
#include "qejdbexception.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //Create an instance of the service object
    //QEjdbDatabaseService service;


    try {
        //QTest::qExec(new Tst_QBson, argc, argv);
        QTest::qExec(new Tst_Connection, argc, argv);
        //QTest::qExec(new Tst_Collection(), argc, argv);
        //QTest::qExec(new Tst_Performance, argc, argv);
        //QTest::qExec(new Tst_Worker, argc, argv);
    } catch(QEjdbException &ex) {
        qDebug() << ex;
    }


    //Connect to the server, specifying the port, and the remove service to connect to
    //ReturnValue ret = service.connect();
    //If the connection failes, ret.isError() will be true
    //if(ret.isError())
    /*{
        //You can use ReturnValues in qDebug() statements to quickly print error messages.
        qCritical() << "Failed to connect:" << ret;
        return(1);
    }
    if (service.createCollection("collection").toBool()) {
        service.save("collection", QBsonObject("name", "test"));
    }*/

    //QTest::qExec(new tst_connection, argc, argv);
    //qDebug() << "connection successfull";
    return 0;
}
