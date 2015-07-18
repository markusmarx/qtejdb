#ifndef QBSONMODELSYNC_P_H
#define QBSONMODELSYNC_P_H
#include "qbsonitemmodel_p.h"
#include <QByteArray>
#include "qtejdb/qejdbdatabase.h"
#include "qbson/qbsonobject.h"
#include "qbson/qbsonarray.h"

class QEjdbAbstractSync: public QObject
{
    Q_OBJECT

public:
    explicit QEjdbAbstractSync(QObject *parent = 0)
        : QObject(parent), m_qBsonItemModel(new QBsonItemModel(this))
    {
        QObject::connect(m_qBsonItemModel, &QBsonItemModel::itemInserted,
                         this, &QEjdbAbstractSync::itemInserted);
        QObject::connect(m_qBsonItemModel, &QBsonItemModel::itemRemoved,
                         this, &QEjdbAbstractSync::itemRemoved);
        QObject::connect(m_qBsonItemModel, &QBsonItemModel::itemUpdated,
                         this, &QEjdbAbstractSync::itemUpdated);
        QObject::connect(m_qBsonItemModel, &QBsonItemModel::itemMoved,
                         this, &QEjdbAbstractSync::itemMoved);
    }

    ~QEjdbAbstractSync()
    {
        QObject::disconnect(m_qBsonItemModel, &QBsonItemModel::itemInserted,
                         this, &QEjdbAbstractSync::itemInserted);
        QObject::disconnect(m_qBsonItemModel, &QBsonItemModel::itemRemoved,
                         this, &QEjdbAbstractSync::itemRemoved);
        QObject::disconnect(m_qBsonItemModel, &QBsonItemModel::itemUpdated,
                         this, &QEjdbAbstractSync::itemUpdated);
        QObject::disconnect(m_qBsonItemModel, &QBsonItemModel::itemMoved,
                         this, &QEjdbAbstractSync::itemMoved);
        delete m_qBsonItemModel;
    }

    QBsonItemModel* model() const;
    QBsonItemModel *m_qBsonItemModel;

public slots:
    virtual void itemRemoved(int row, QBsonObject removedObject) = 0;
    virtual void itemInserted(int row) = 0;
    virtual void itemMoved(int sourceRow, int destinationRow) = 0;
    virtual void itemUpdated(QString property, QVariant value, int row) = 0;

};
#endif // QBSONMODELSYNC_P_H

