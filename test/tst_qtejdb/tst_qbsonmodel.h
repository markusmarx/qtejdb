#ifndef TST_QBSONMODEL_H
#define TST_QBSONMODEL_H

#include <QObject>
#include "tst_base.h"

class Tst_QBsonModel: public QObject, public Tst_Base
{
    Q_OBJECT
public:
    Tst_QBsonModel(QObject *parent = 0);
    ~Tst_QBsonModel();

public slots:

    void itemRemoved(QBsonObject removedObject);
    void itemInserted(int row);
    void itemMoved(int sourceRow, int destinationRow);
    void itemUpdated(int row);

private slots:
    void initTestCase();
    void tst_QBsonModelSimple();

protected:
    QBsonObject createTestBsonObject(int marker);
    void testBsonObjectMarker(QBsonObject bsonObject, int marker);

private:
    int m_emitItemInserted;
    int m_emitItemUpdated;
    int m_emitItemMovedSourceRow;
    int m_emitItemMovedDestinationRow;
    QBsonObject m_emitItemRemoved;
};

#endif // TST_QBSONMODEL_H
