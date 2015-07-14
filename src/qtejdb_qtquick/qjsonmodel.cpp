#include "qjsonmodel.h"
#include "qejdbclient.h"

QJsonModel::QJsonModel(QObject *parent)
    :QEjdbItemModel(parent)
{

}

QJsonModel::~QJsonModel()
{

}

/**
 * @brief Returns the client that connects to the db.
 * @return
 */
QEjdbClient *QJsonModel::client() const
{
    return m_client;
}

/**
 * @brief Sets a new client.
 */
void QJsonModel::setClient(QEjdbClient *client)
{
    if (m_client != client) {
        m_client = client;
    }
    emit clientChanged();
}

