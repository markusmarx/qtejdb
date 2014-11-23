#include <QtCore/QCoreApplication>
#include "qejdbdatabaseservice.h"
#include "serverconfiguration.h"
#include "rcpsocketlistener.h"
#include <QtArg/Arg>
#include <QtArg/CmdLine>
#include <QtArg/Help>
#include <Server>
#include <ServerProtocolListenerTcp>
#include <QDebug>

ServerConfiguration *parseCmd(QCoreApplication *app, int argc, char *argv[]);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ServerConfiguration *config = parseCmd(&a, argc, argv);

    if (!config) {
        return 0;
    }

    if (config->run()) {
        QObject::connect(config, &ServerConfiguration::shutdown, &a, &QCoreApplication::quit);

        return a.exec();
    } else {
        return -1;
    }

}

/**
 * @brief parseCmd parse commandline args.
 * @param argc
 * @param argv
 *
 * @return configuration instance
 */
ServerConfiguration *parseCmd(QCoreApplication *app, int argc, char *argv[])
{
    QtArgCmdLine cmd( argc, argv );
    QtArg configFile( QLatin1Char('c'), QLatin1String( "config" ),
                            QLatin1String( "Path to configuration file."), false, true );
    configFile.setValueSpecifier(QLatin1String("path"));

    QtArg port(QLatin1Char('p'), QLatin1String("port"),
               QLatin1String("websocket port"), true, true);
    port.setValueSpecifier("port");

    QtArg dbName(QLatin1Char('n'), QLatin1String("dbname"),
               QLatin1String("database name"), true, true);
    dbName.setValueSpecifier("name");
    QtArg dbPath(QLatin1Char('P'), QLatin1String("dbpath"),
               QLatin1String("path where the db is created."), true, true);
    dbPath.setValueSpecifier("path");


    QtArgHelp help( &cmd );
    help.printer()->setProgramDescription(
        QLatin1String( "Show help." ) );
    help.printer()->setExecutableName( QLatin1String( argv[0] ) );

    cmd.addArg(configFile);
    cmd.addArg(help);
    //cmd.addArg(port);
    cmd.addArg(dbName);
    cmd.addArg(dbPath);

    try {

        cmd.parse();

    }catch( const QtArgHelpHasPrintedEx & x ) {
        return 0;
    }

    catch( const QtArgBaseException & x ) {
       qDebug() << x.what();
       return 0;
    }

    ServerConfiguration *config = new ServerConfiguration(app);
    config->setObjectName("serverConfig");
    config->setDatabaseName(dbName.value().toString());
    config->setDatabasePath(dbPath.value().toString());
    RcpSocketListener *wsListener = new RcpSocketListener(config);
    config->addServerListener(wsListener);

    return config;
}
