#ifndef HELPER_H
#define HELPER_H
#include "qbson/qbsonobject.h"
#include "qbson/qbsonarray.h"
#include "qbson/qbsonoid.h"

QBsonObject createBsonObject(bool withId, int marker = -1);

QList<QBsonObject> createBsonObjects(uint count);
#endif // HELPER_H

