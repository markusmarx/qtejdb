#ifndef QEJDBCLIENT_H
#define QEJDBCLIENT_H

#include <QObject>
#include <QJSValue>
#include <QQmlParserStatus>

#include "qejdbdatabase.h"

class QEjdbClient : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(QString uri READ uri WRITE setUri NOTIFY uriChanged)
    Q_PROPERTY(QString connectionName READ connectionName WRITE setConnectionName NOTIFY connectionNameChanged)

public:
    explicit QEjdbClient(QObject *parent = 0);
    ~QEjdbClient();

    void classBegin();
    void componentComplete();

    Q_INVOKABLE QJSValue save(QString collectionName, const QJSValue &jsValue);
    Q_INVOKABLE QJSValue load(QString collectionName, QString uid);

    QString uri() const;
    QString connectionName() const;

signals:

    void uriChanged(QString arg);
    void connectionNameChanged(QString arg);

public slots:

    void connect();
    void disconnect();
    void setConnectionName(QString arg);
    void setUri(QString arg);

private:

    /**
     * @brief m_connectionName stores the connectionName that stores the database
     * in dictionary @see QEjdbDatabase::addDatabase()
     */
    QString m_connectionName;

    /**
     * @brief m_uri is the connection string @see QEjdbDatabase::addDatabase()
     */
    QString m_uri;

    inline QEjdbDatabase database()
    {
        return QEjdbDatabase::database(m_connectionName);
    }

    inline void checkCollection(QEjdbDatabase &db, QString collection)
    {
        if (!db.containsCollection(collection)) {
            db.createCollection(collection);
        }
    }
};

#endif // QEJDBCLIENT_H
