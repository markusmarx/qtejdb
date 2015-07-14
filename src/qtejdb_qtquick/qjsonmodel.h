#ifndef QJSONMODEL_H
#define QJSONMODEL_H
#include "qejdbitemmodel.h"

QT_FORWARD_DECLARE_CLASS(QEjdbClient)

class QJsonModel: public QEjdbItemModel
{

    Q_PROPERTY(QEjdbClient* client READ client WRITE setClient NOTIFY clientChanged)
    Q_OBJECT
public:
    QJsonModel(QObject *parent);
    ~QJsonModel();

    QEjdbClient *client() const;
public slots:
    void setClient(QEjdbClient *client);

signals:
    void clientChanged();

private:
    QEjdbClient* m_client;
};

#endif // QJSONMODEL_H
