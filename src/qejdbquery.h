#ifndef QEJDBQUERY_H
#define QEJDBQUERY_H
#include <QList>

class QEjdbQueryData;
class QEjdbCollection;
class QBsonObject;
class QEjdbQuery
{
public:

    QEjdbQuery(const QEjdbCollection& query);
    QEjdbQuery(const QEjdbQuery &);
    QEjdbQuery &operator=(const QEjdbQuery &);
    ~QEjdbQuery();

    QList<QBsonObject> exec(const QBsonObject& query);

    QString collectionName() const;
private:
    QEjdbQueryData* data;
};

#endif // QEJDBQUERY_H
