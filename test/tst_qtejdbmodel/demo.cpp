#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "qejdbitemmodel.h"
#include "qejdbdatabase.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    engine.addImportPath("/home/markus/development/cpp.labs/qtejdb-build/plugins/");
    //QEjdbDatabase::removeDatabaseFiles(".", "testmodel_db");

    QEjdbDatabase db = QEjdbDatabase::addDatabase("file:testmodel_db");
    db.open();
    db.createCollection("modeltest");

    /*for (int i = 0; i < 1; i++) {
        QBsonObject obj;
        obj.append("name1", "Markus")
                .append("name2", "Marx");
        db.save("modeltest", obj);
    }*/

    QEjdbItemModel *itemModel = new QEjdbItemModel(db);
    itemModel->setCollection("modeltest");
    QStringList ll;
    ll << "name1" << "name2";
    itemModel->setNames(ll, QHash<QString, QString>());

    engine.rootContext()->setContextProperty(
                "qejdbmodel", itemModel);
    engine.load(QUrl("qrc:/Demo.qml"));
    return app.exec();
}
