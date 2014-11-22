#include "tst_connection.h"
#include "qejdbdatabase.h"
#include "bson/qbsonobject.h"
Tst_Connection::Tst_Connection(QObject *parent) :
    QObject(parent)
{
}

void Tst_Connection::tst_simple()
{

    QEjdbDatabase db = QEjdbDatabase::addDatabase(
                "socket:qtejdbtest", 0);

    db.open();
    Q_ASSERT(db.isOpen());


    Q_ASSERT(!db.containsCollection("testCollection"));
    Q_ASSERT(db.createCollection("testCollection"));
    Q_ASSERT(db.containsCollection("testCollection"));


    QBsonObject bsonObj;
    bsonObj.insert("test", QBsonValue("test"));

    Q_ASSERT(db.save("testCollection", bsonObj));
    Q_ASSERT(bsonObj.contains("_id"));
    Q_ASSERT(bsonObj.value("_id").toId().isValid());

    Q_ASSERT(db.removeCollection("testCollection"));
    Q_ASSERT(!db.containsCollection("testCollection"));
}
