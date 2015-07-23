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
    Q_PROPERTY(int count READ count)

public:
    BaseModel(QObject *parent = 0);
    QEjdbClient* client() const;

    QString collection() const;

    int count() const;

public slots:
    void setClient(QEjdbClient* client);
    virtual void connected() = 0;
    virtual void disconnected() = 0;
    void setCollection(QString collection);
    void insert(int row, QJSValue value);
    void set(int row, QJSValue value);
    QJSValue get(int row);
    void append(QJSValue value);
    void setProperty(int row, QString property, QJSValue value);

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
