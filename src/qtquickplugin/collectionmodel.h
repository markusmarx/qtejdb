#ifndef COLLECTIONMODEL_H
#define COLLECTIONMODEL_H
#include "itemmodel/qejdbitemmodel.h"
#include <QJSValue>
#include <QQmlParserStatus>

QT_FORWARD_DECLARE_CLASS(QEjdbClient)

class CollectionModel: public QEjdbItemModel, public QQmlParserStatus
{
    Q_OBJECT

    Q_PROPERTY(QEjdbClient* client READ client WRITE setClient NOTIFY clientChanged)
    Q_PROPERTY(QJSValue query READ query WRITE setQuery NOTIFY queryChanged)
    Q_PROPERTY(QJSValue hints READ hints WRITE setHints NOTIFY hintsChanged)
    Q_PROPERTY(QString collection READ collection WRITE setCollection NOTIFY collectionChanged)

public:
    CollectionModel(QObject *parent);
    ~CollectionModel();
    void classBegin();
    void componentComplete();
    QEjdbClient *client() const;
    QJSValue query() const;
    QJSValue hints() const;
    QString collection() const;

public slots:
    void setClient(QEjdbClient *client);
    void setQuery(QJSValue query);
    void setHints(QJSValue hints);
    void setCollection(QString collection);
    void insert(QJSValue value, int row);

signals:
    void clientChanged();
    void queryChanged(QJSValue query);
    void hintsChanged(QJSValue hints);
    void collectionChanged(QString collection);

private:
    QEjdbClient* m_client;
    QJSValue m_query;
    QJSValue m_hints;
    QString m_collection;
};

#endif // COLLECTIONMODEL_H
