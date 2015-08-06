#ifndef QEJDBCLIENT_H
#define QEJDBCLIENT_H

#include <QObject>
#include <QJSValue>
#include <QQmlParserStatus>

#include "qtejdb/qejdbdatabase.h"
#include "qbsonconverter.h"

QT_FORWARD_DECLARE_CLASS(BaseModel)
QT_FORWARD_DECLARE_CLASS(QEjdbClientPrivate)
class Q_DECL_EXPORT QEjdbClient : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(QString uri READ uri WRITE setUri NOTIFY uriChanged)
    Q_PROPERTY(QString connectionName READ connectionName WRITE setConnectionName NOTIFY connectionNameChanged)
    Q_PROPERTY(bool autoCreateCollection READ autoCreateCollection WRITE setAutoCreateCollection NOTIFY autoCreateCollectionChanged)

public:
    explicit QEjdbClient(QObject *parent = 0);
    ~QEjdbClient();

    void classBegin();
    void componentComplete();

    QString uri() const;
    QString connectionName() const;
    bool autoCreateCollection() const;
    QBsonValue convert(const QJSValue &jsValue);
    QJSValue convert(const QBsonObject &bsonObject);
    bool isConnected();
    void registerModel(BaseModel *baseModel);

signals:

    void uriChanged(QString arg);
    void connectionNameChanged(QString arg);
    void autoCreateCollectionChanged(bool arg);
    void connected();
    void disconnected();

public slots:

    void setConnectionName(QString arg);
    void setUri(QString arg);
    void setAutoCreateCollection(bool arg);
    QJSValue load(QString collectionName, QJSValue uid);
    QJSValue save(QString collectionName, const QJSValue &jsValue);
    QJSValue remove(QString collectionName, QJSValue uid);
    QJSValue query(QString collection, QJSValue query, QJSValue hints);
    void createCollection(QString collection);
    void removeCollection(QString collection);
    void connect();
    void disconnect();
private:



    Q_DECLARE_PRIVATE(QEjdbClient)
    QEjdbClientPrivate *d_ptr;

};

#endif // QEJDBCLIENT_H
