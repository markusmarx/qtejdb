#include "qejdbplugin.h"

#include <QtQml>
#include "qejdbclient.h"
#include "collectionmodel.h"
#include "arraypropertymodel.h"

/*!
 \class QEjdbPlugin
 \brief teset
 */

QEjdbPlugin::QEjdbPlugin(QObject *parent) :  QQmlExtensionPlugin(parent)
{

}

QEjdbPlugin::~QEjdbPlugin()
{

}


void QEjdbPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("QtEjdb"));
    qmlRegisterType<QEjdbClient>(uri, 1, 0, "Client");
    qmlRegisterType<CollectionModel>(uri, 1, 0, "CollectionModel");
    qmlRegisterType<ArrayPropertyModel>(uri, 1, 0, "ArrayPropertyModel");

}


void QEjdbPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{

}
