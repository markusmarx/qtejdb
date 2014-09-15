#ifndef TST_COLLECTION_H
#define TST_COLLECTION_H

#include <QObject>


class Tst_Collection : public QObject
{
    Q_OBJECT
public:
    explicit Tst_Collection(QObject *parent = 0);

signals:

private slots:
    void initTestCase();
    void tst_simpleCreate();
    void tst_simpleQuery();
    void tst_dataTypes();
    void cleanupTestCase();

private:


};

#endif // TST_COLLECTION_H
