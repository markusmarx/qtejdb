#ifndef QEJDBCLIENT_H
#define QEJDBCLIENT_H

#include <QObject>
#include <QJSValue>
#include <QQmlParserStatus>

#include "qejdbdatabase.h"
#include "qbsonconverter.h"

class QEjdbClientPrivate;
class QEjdbClient : public QObject, public QQmlParserStatus
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

    Q_INVOKABLE QJSValue save(QString collectionName, const QJSValue &jsValue);
    Q_INVOKABLE QJSValue load(QString collectionName, QJSValue uid);

    QString uri() const;
    QString connectionName() const;
    bool autoCreateCollection() const;

signals:

    void uriChanged(QString arg);
    void connectionNameChanged(QString arg);
    void autoCreateCollectionChanged(bool arg);

public slots:

    void connect();
    void disconnect();
    void setConnectionName(QString arg);
    void setUri(QString arg);
    void setAutoCreateCollection(bool arg);

private:

    QBsonObject convert(const QJSValue &jsValue);
    QJSValue convert(const QBsonObject &bsonObject);

    Q_DECLARE_PRIVATE(QEjdbClient)
    QEjdbClientPrivate *d_ptr;

};

#endif // QEJDBCLIENT_H
