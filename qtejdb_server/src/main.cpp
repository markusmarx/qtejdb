#include <QtCore/QCoreApplication>
#include "echoserver.h"
#include "serverconfiguration.h"
#include "websocketlistener.h"
#include <QtArg/Arg>
#include <QtArg/CmdLine>
#include <QtArg/Help>

#include <QDebug>

ServerConfiguration *parseCmd(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    ServerConfiguration *config = parseCmd(argc, argv);

    if (!config) {
        return 0;
    }

    QCoreApplication a(argc, argv);
    config->run();
    QObject::connect(config, &ServerConfiguration::shutdown, &a, &QCoreApplication::quit);
    return a.exec();

}

/**
 * @brief parseCmd parse commandline args.
 * @param argc
 * @param argv
 *
 * @return configuration instance
 */
ServerConfiguration *parseCmd(int argc, char *argv[])
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
    cmd.addArg(port);
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

    ServerConfiguration *config = new ServerConfiguration;
    config->setDatabaseName(dbName.value().toString());
    config->setDatabasePath(dbPath.value().toString());
    WebSocketListener *wsListener = new WebSocketListener(port.value().toInt(), config);
    config->addServerListener(wsListener);

    return config;
}
