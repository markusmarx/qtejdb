#ifndef BASEMODEL_H
#define BASEMODEL_H
#include "itemmodel/qejdbitemmodel.h"
#include <QObject>
#include <QJSValue>
#include "qejdbclient.h"

class BaseModel: public QEjdbItemModel
{
    Q_OBJECT

    Q_PROPERTY(QEjdbClient* client READ client WRITE setClient NOTIFY clientChanged)
    Q_PROPERTY(QString collection READ collection WRITE setCollection NOTIFY collectionChanged)

public:
    BaseModel(QObject *parent = 0);
    QEjdbClient* client() const;

    QString collection() const;

public slots:
    void setClient(QEjdbClient* client);
    virtual void connected() = 0;
    virtual void disconnected() = 0;
    void setCollection(QString collection);
    void insert(QJSValue value, int row);

signals:
    void clientChanged(QEjdbClient* client);
    void collectionChanged(QString collection);
    void fetch();

protected:
    QEjdbClient* m_client;
    QString m_collection;

    virtual void tryInit() = 0;

};

#endif // BASEMODEL_H
