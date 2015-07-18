#ifndef QEJDBCOLLECTIONSYNC_H
#define QEJDBCOLLECTIONSYNC_H
#include "qtejdb/qejdbdatabase.h"
#include "qbson/qbsonobject.h"
#include "qbsonitemmodelsync_p.h"

class QEjdbCollectionSync: public QEjdbAbstractSync
{
    Q_OBJECT
public:
    explicit QEjdbCollectionSync(QEjdbDatabase db, QObject *parent = 0);
    ~QEjdbCollectionSync();
    QBsonObject query() const;
    QBsonObject hints() const;
    QString collection() const;

public slots:
    void fetch();
    void setQuery(QBsonObject query);
    void setHints(QBsonObject hints);
    void setCollection(QString collection);
    void itemRemoved(int row, QBsonObject removedObject);
    void itemInserted(int row);
    void itemUpdated(QString property, QVariant value, int row);
    void itemMoved(int sourceRow, int destinationRow);

private:
    QEjdbDatabase m_db;
    QString m_collection;
    QBsonObject m_query;
    QBsonObject m_hints;

    bool isDbValid()
    {
        return m_db.isOpen()
                && m_db.containsCollection(m_collection);
    }
};

#endif // QEJDBCOLLECTIONSYNC_H
