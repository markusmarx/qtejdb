#ifndef QEJDBPLUGIN_H
#define QEJDBPLUGIN_H

#include <QQmlExtensionPlugin>

class Q_DECL_EXPORT QEjdbPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    explicit QEjdbPlugin(QObject *parent=0);
    ~QEjdbPlugin();

    void registerTypes(const char *uri);
    void initializeEngine(QQmlEngine * engine, const char * uri);
};

#endif // QEJDBPLUGIN_H
