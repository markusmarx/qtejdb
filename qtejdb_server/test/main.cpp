#include <QCoreApplication>
#include "tst_connection.h"
#include "qejdbdatabaseservice.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //Create an instance of the service object
    QEjdbDatabaseService service;

    //Connect to the server, specifying the port, and the remove service to connect to
    ReturnValue ret = service.connect("tcp://localhost:10123/QEjdbDatabase");
    //If the connection failes, ret.isError() will be true
    if(ret.isError())
    {
        //You can use ReturnValues in qDebug() statements to quickly print error messages.
        qCritical() << "Failed to connect:" << ret;
        return(1);
    }
    if (service.createCollection("collection").toBool()) {
        service.save("collection", QBsonObject("name", "test"));
    }

    //QTest::qExec(new tst_connection, argc, argv);
    qDebug() << "connection successfull";
    return 0;
}
