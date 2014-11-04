#ifndef TST_COLLECTION_H
#define TST_COLLECTION_H

#include <QObject>
class QBsonObject;

class Tst_Collection : public QObject
{
    Q_OBJECT
public:
    explicit Tst_Collection(QObject *parent = 0);

    QBsonObject createTestObject();
signals:

private slots:
    void initTestCase();
    void tst_simpleCRUD();
    void tst_simpleQuery();
    void tst_dataTypes();
    void cleanupTestCase();

private:


};

#endif // TST_COLLECTION_H
