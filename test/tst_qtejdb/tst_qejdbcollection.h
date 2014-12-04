#ifndef TST_QEJDBCOLLECTION_H
#define TST_QEJDBCOLLECTION_H

#include <QObject>
class QBsonObject;

class Tst_QEjdbCollection : public QObject
{
    Q_OBJECT
public:
    explicit Tst_QEjdbCollection(QString url, QObject *parent = 0);

    QBsonObject createTestObject();
signals:

private slots:
    void initTestCase();
    void tst_simpleCRUD();
    void tst_simpleQuery();
    void tst_dataTypes();
    void tst_joins();
    void cleanupTestCase();

private:
    QString m_url;


};

#endif // TST_QEJDBCOLLECTION_H
