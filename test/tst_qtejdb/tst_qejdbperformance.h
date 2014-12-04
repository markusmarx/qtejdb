#ifndef Tst_QEjdbPerformance_H
#define Tst_QEjdbPerformance_H

#include <QObject>

class Tst_QEjdbPerformance : public QObject
{
    Q_OBJECT
public:
    explicit Tst_QEjdbPerformance(QString url, QObject *parent = 0);

signals:

private slots:

    void initTestCase();

    void cleanupTestCase();

    void tst_insQryTsd();

private:
    QString m_url;
};

#endif // Tst_QEjdbPerformance_H
