#ifndef TST_QBSONCONVERTER_H
#define TST_QBSONCONVERTER_H

#include <QObject>
#include "qbsonconverter.h"

class tst_QBsonConverter : public QObject, public QBsonConverter
{
    Q_OBJECT
public:
    explicit tst_QBsonConverter(QObject *parent = 0);
    ~tst_QBsonConverter();

signals:

private slots:
    void initTestCase();
    void tst_simplePropertyTypesFromJS();
    void tst_simplePropertyTypesFromBson();
};

#endif // TST_QBSONCONVERTER_H
