#ifndef TST_DATABASE_H
#define TST_DATABASE_H

#include <QObject>
#include "qejdbdatabase.h"

class Tst_Database : public QObject
{
    Q_OBJECT
public:
    explicit Tst_Database(QObject *parent = 0);

signals:

private Q_SLOTS:

    void tst_open();

    /**
     * @brief tst_openWithDir Open the database in a subdirctory.
     */
    void tst_openWithDir();

    void tst_collection();

    void cleanupTestCase();
private:
    QEjdbDatabase m_db2;
};

#endif // TST_DATABASE_H