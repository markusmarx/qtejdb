#ifndef TST_PERFORMANCE_H
#define TST_PERFORMANCE_H

#include <QObject>

class Tst_Performance : public QObject
{
    Q_OBJECT
public:
    explicit Tst_Performance(QObject *parent = 0);

signals:

private slots:

    void initTestCase();

    void cleanupTestCase();

    void tst_insQryTsd();
};

#endif // TST_PERFORMANCE_H
