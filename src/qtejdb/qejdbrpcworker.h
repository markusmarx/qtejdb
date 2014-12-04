#ifndef QEJDBRPCWORKER_H
#define QEJDBRPCWORKER_H
#include "qejdbworker.h"
#include <ClientProxy>
#include "qbsonobject.h"

class QEjdbRpcClientService: public QtRpc::ClientProxy
{
    Q_OBJECT

    QTRPC_CLIENTPROXY(QEjdbRpcClientService)

public:
    explicit QEjdbRpcClientService(QObject *parent = 0);

signals:

    ReturnValue save(QString collection, QBsonObject obj);
    ReturnValue createCollection(QString collectionName);
    ReturnValue removeCollection(QString collectionName);
    ReturnValue containsCollection(QString collectionName);
    ReturnValue load(const QString &collectionName, const QString &oid);
    ReturnValue remove(const QString &collectionName, const QString &oid);
    ReturnValue query(const QString &collectionName, const QBsonObject &query);

public slots: 

};

class QEjdbRpcWorker: public QEjdbWorker
{
public:

    /**
     * @brief QEjdbRpcWorker
     * @param url
     * @param mode
     */
    explicit QEjdbRpcWorker(const QUrl &url, int mode);

    ~QEjdbRpcWorker();

    /**
     * @brief open connect to the server
     *
     * @return true if successfull otherwise false.
     */
    virtual void open();

    virtual bool isOpen();
    virtual bool close();

    virtual bool containsCollection(const QString &collectionName);
    virtual bool createCollection(const QString &collectionName);
    virtual bool removeCollection(const QString &collectionName);

    virtual bool save(const QString &collectionName, QBsonObject &bson);
    virtual QBsonObject load(const QString &collectionName, const QString &oid);
    virtual bool remove(const QString &collectionName, const QString &oid);

    virtual QEjdbResult query(const QString &collectionName, const QBsonObject &query);

private:
    QEjdbRpcClientService *m_clientService;

    QUrl m_url;

    void checkReturnValue(const ReturnValue &val);
};

QTRPC_REGISTER_METATYPE(QBsonObject)
QTRPC_REGISTER_METATYPE(QList<QBsonObject>)
QTRPC_REGISTER_METATYPE(QEjdbResult)


#endif // QEJDBRPCWORKER_H
