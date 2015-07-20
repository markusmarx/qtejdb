#include "qejdbplugin.h"

#include <QtQml>
#include "qejdbclient.h"
#include "collectionmodel.h"

QEjdbPlugin::QEjdbPlugin(QObject *parent) :  QQmlExtensionPlugin(parent)
{

}

QEjdbPlugin::~QEjdbPlugin()
{

}

/**
 * @brief QEjdbPlugin::registerTypes
 * @param uri
 */
void QEjdbPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("QEjdb"));
    qmlRegisterType<QEjdbClient>(uri, 1, 0, "Client");
    qmlRegisterType<CollectionModel>(uri, 1, 0, "CollectionModel");

}


void QEjdbPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{

}
