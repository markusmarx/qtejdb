#ifndef TST_DATABASE_H
#define TST_DATABASE_H

#include <QObject>

class Tst_Database : public QObject
{
    Q_OBJECT
public:
    explicit Tst_Database(QObject *parent = 0);

signals:

private Q_SLOTS:

    void tst_open();

    void tst_collection();

    void cleanupTestCase();

};

#endif // TST_DATABASE_H
