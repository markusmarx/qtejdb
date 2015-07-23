#ifndef COLLECTIONMODEL_H
#define COLLECTIONMODEL_H
#include "basemodel.h"
#include <QJSValue>
#include <QQmlParserStatus>

QT_FORWARD_DECLARE_CLASS(QEjdbClient)

class CollectionModel: public BaseModel, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(QJSValue query READ query WRITE setQuery NOTIFY queryChanged)
    Q_PROPERTY(QJSValue hints READ hints WRITE setHints NOTIFY hintsChanged)

public:
    CollectionModel(QObject *parent = 0);
    ~CollectionModel();
    void classBegin();
    void componentComplete();

    QJSValue query() const;
    QJSValue hints() const;

public slots:

    void setQuery(QJSValue query);
    void setHints(QJSValue hints);
    void connected();
    void disconnected();

signals:
    void queryChanged(QBsonObject query);
    void hintsChanged(QBsonObject hints);

protected:
    bool checkProperties();

    void tryInit();


private:
    QJSValue m_query;
    QJSValue m_hints;
};

#endif // COLLECTIONMODEL_H
