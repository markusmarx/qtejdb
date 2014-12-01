#ifndef TST_QEJDBDATABASE_H
#define TST_QEJDBDATABASE_H

#include <QObject>
#include "qejdbdatabase.h"

class Tst_QEjdbDatabase : public QObject
{
    Q_OBJECT
public:
    explicit Tst_QEjdbDatabase(QObject *parent = 0);

signals:

private Q_SLOTS:

    void tst_open();
    void tst_openWithDir();
    void tst_collection();
    void tst_workers();
    void cleanupTestCase();

private:
    QEjdbDatabase m_db2;
};

#endif // TST_QEJDBDATABASE_H
