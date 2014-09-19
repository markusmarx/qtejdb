#ifndef TST_QBSON_H
#define TST_QBSON_H
#include <QObject>

class Tst_QBson: public QObject
{
    Q_OBJECT
public:
    explicit Tst_QBson(QObject *parent = 0):QObject(parent)
    {

    }


private Q_SLOTS:

    void tst_list();
    void tst_qbson();
    void tst_stream();

};

#endif // TST_QBSON_H
