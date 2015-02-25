#include "qejdbplugin.h"

#include <QtQml>
#include "qejdbclient.h"

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
    qmlRegisterType<QEjdbClient>(uri, 1, 0, "QEjdbClient");

}


void QEjdbPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{

}
