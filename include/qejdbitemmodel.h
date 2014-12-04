#ifndef QEJDBITEMMODEL_H
#define QEJDBITEMMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include "qejdbdatabase.h"
#include "qbsonobject.h"

class QEjdbItemModelWorker;

class QEjdbItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit QEjdbItemModel(QEjdbDatabase db = QEjdbDatabase(), QObject *parent = 0);

    ~QEjdbItemModel();

     QVariant data(const QModelIndex &index, int role) const;

     QModelIndex index(int row, int column,
                       const QModelIndex &parent = QModelIndex()) const;

     QModelIndex parent(const QModelIndex &index) const;

     int rowCount(const QModelIndex &parent = QModelIndex()) const;
     int columnCount(const QModelIndex &parent = QModelIndex()) const;

     Qt::ItemFlags flags(const QModelIndex &index) const;

     bool setData(const QModelIndex &index, const QVariant &value,
                  int role = Qt::EditRole);

     bool insertRows(int position, int rows,
                     const QModelIndex &parent = QModelIndex());

     bool removeRows(int position, int rows,
                     const QModelIndex &parent = QModelIndex());

     void setNames(QStringList names, QHash<QString, QString> namesMap);

     void setCollection(QString collectionName);
signals:

public slots:

private:
    QEjdbDatabase m_db;

    QBsonObject getQBson(const QModelIndex &index) const;

    QHash<int, QByteArray> m_roleNames;

    QHash<int, QByteArray> m_roleNamesMap;

    QEjdbItemModelWorker *m_modelWorker;
};

#endif // QEJDBITEMMODEL_H
