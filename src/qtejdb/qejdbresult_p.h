#ifndef QEJDBRESULT_P_H
#define QEJDBRESULT_P_H
#include "qbson/qbsonobject.h"
#include <QLinkedList>

class QEjdbResultData  {

public:
    QEjdbResultData()
    {
        ref = 1;
        currentIt = values.begin();
    }


    QLinkedList<QVariant> values;
    QLinkedList<QVariant>::iterator currentIt;
    QAtomicInt ref;

    QBsonObject nextBsonValue();
    QBsonObject firstBsonValue();

    friend class QBsonObject;

};


#endif // QEJDBRESULT_P_H
