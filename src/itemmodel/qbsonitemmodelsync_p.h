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
    explicit QEjdbAbstractSync(QObject *parent = 0);

    ~QEjdbAbstractSync();

    QBsonItemModel* model() const;


public slots:
    virtual void itemRemoved(int row, QBsonObject removedObject) = 0;
    virtual void itemInserted(int row) = 0;
    virtual void itemMoved(int sourceRow, int destinationRow) = 0;
    virtual void itemUpdated(QString property, QVariant value, int row) = 0;
    virtual void fetch() = 0;

signals:
    void reset();

protected:
    QBsonItemModel *m_qBsonItemModel;

};
#endif // QBSONMODELSYNC_P_H

