#ifndef TST_BASE_H
#define TST_BASE_H

#include <QObject>
#include "qbsonobject.h"
class Tst_Base
{

public:
    explicit Tst_Base();
    ~Tst_Base();

signals:

public slots:
protected:
    QBsonObject createTestBsonObject(bool withId = false);
    QList<QBsonObject> createTestBsonObjectList(int count, bool withId = false);
    void testBsonObject(QBsonObject testObj);
};

#endif // TST_BASE_H
