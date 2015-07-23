#include "qbsonitemmodelsync_p.h"

QEjdbAbstractSync::QEjdbAbstractSync(QObject *parent)
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
    QObject::connect(m_qBsonItemModel, &QBsonItemModel::reset,
                     this, &QEjdbAbstractSync::reset);
}

QEjdbAbstractSync::~QEjdbAbstractSync()
{
    QObject::disconnect(m_qBsonItemModel, &QBsonItemModel::itemInserted,
                        this, &QEjdbAbstractSync::itemInserted);
    QObject::disconnect(m_qBsonItemModel, &QBsonItemModel::itemRemoved,
                        this, &QEjdbAbstractSync::itemRemoved);
    QObject::disconnect(m_qBsonItemModel, &QBsonItemModel::itemUpdated,
                        this, &QEjdbAbstractSync::itemUpdated);
    QObject::disconnect(m_qBsonItemModel, &QBsonItemModel::itemMoved,
                        this, &QEjdbAbstractSync::itemMoved);
    QObject::disconnect(m_qBsonItemModel, &QBsonItemModel::reset,
                     this, &QEjdbAbstractSync::reset);
    delete m_qBsonItemModel;
}

QBsonItemModel *QEjdbAbstractSync::model() const
{
    return m_qBsonItemModel;
}
