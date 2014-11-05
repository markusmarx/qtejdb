#ifndef QEJDBQUERY_H
#define QEJDBQUERY_H
#include <QList>

class QEjdbQueryData;
class QEjdbDatabase;
class QBsonObject;
class QEjdbQuery
{
public:

    QEjdbQuery(const QString &collectionName, const QEjdbDatabase& query);
    QEjdbQuery(const QEjdbQuery &);
    QEjdbQuery &operator=(const QEjdbQuery &);
    ~QEjdbQuery();

    QList<QBsonObject> exec(const QBsonObject& query);

    QString collectionName() const;
private:
    QEjdbQueryData* data;
};

#endif // QEJDBQUERY_H
