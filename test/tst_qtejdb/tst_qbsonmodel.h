#ifndef TST_QBSONMODEL_H
#define TST_QBSONMODEL_H

#include <QObject>
#include "tst_base.h"

class Tst_QBsonModel: public QObject, public Tst_Base
{
    Q_OBJECT
public:
    Tst_QBsonModel(QString url, QObject *parent = 0);
    ~Tst_QBsonModel();

public slots:

    void itemRemoved(int row, QBsonObject removedObject);
    void itemInserted(int row);
    void itemMoved(int sourceRow, int destinationRow);
    void itemUpdated(QString property, QVariant value, int row);

private slots:
    void initTestCase();
    void tst_QBsonModelSimple();
    void tst_QEjdbCollectionSync();
    void tst_QEjdbArrayPropertySync();

protected:
    QBsonObject createTestBsonObject(int marker);
    void testBsonObjectMarker(QBsonObject bsonObject, int marker);

private:
    int m_emitItemInserted;
    int m_emitItemUpdated;
    QString m_emitItemUpdatedProperty;
    QVariant m_emitItemUpdatedValue;
    int m_emitItemMovedSourceRow;
    int m_emitItemMovedDestinationRow;
    QBsonObject m_emitItemRemoved;
    QString m_url;
};

#endif // TST_QBSONMODEL_H
