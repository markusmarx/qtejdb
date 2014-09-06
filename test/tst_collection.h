#ifndef TST_COLLECTION_H
#define TST_COLLECTION_H

#include <QObject>

class Tst_Collection : public QObject
{
    Q_OBJECT
public:
    explicit Tst_Collection(QObject *parent = 0);

signals:

public slots:

    void tst_simpleCreate();

};

#endif // TST_COLLECTION_H
