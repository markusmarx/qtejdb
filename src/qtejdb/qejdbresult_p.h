#ifndef QEJDBRESULT_P_H
#define QEJDBRESULT_P_H
#include "qbsonobject.h"

class QEjdbResultData  {

public:
    QEjdbResultData()
    {
        ref = 1;
    }


    QList<QBsonObject> values;
    QAtomicInt ref;

    friend class QBsonObject;

};


#endif // QEJDBRESULT_P_H
