#ifndef QEJDBITEMMODEL_H
#define QEJDBITEMMODEL_H
/*
 * Copyright (C) 2014-2015 Markus Marx <markus.marx@marxenter.de>
 *
 * This file is part of the qejdb.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 */
#include <QObject>
#include "qtejdb/qejdbdatabase.h"
#include "qbson/qbsonobject.h"
#include <QAbstractListModel>
#include "qbsonitemmodelsync_p.h"
QT_FORWARD_DECLARE_CLASS(QBsonItemModel)

class Q_DECL_EXPORT QEjdbItemModel : public QAbstractListModel
{
    Q_OBJECT
    
public:
    explicit QEjdbItemModel(QEjdbAbstractSync *sync, QObject *parent = 0);
    explicit QEjdbItemModel(QObject *parent = 0);

    ~QEjdbItemModel();

     QVariant data(const QModelIndex &index, int role) const;

     int rowCount(const QModelIndex &parent = QModelIndex()) const;
     bool setData(const QModelIndex &index, const QVariant &value,
                  int role = Qt::EditRole);
     bool insertRow(int row, const QModelIndex &parent);
     bool insertRows(int position, int rows,
                     const QModelIndex &parent = QModelIndex());
     bool removeRows(int position, int rows,
                     const QModelIndex &parent = QModelIndex());

     QVariant headerData(int section, Qt::Orientation orientation, int role) const;

     void setSync(QEjdbAbstractSync *sync);
     QHash<int, QByteArray> roleNames() const;
     int columnCount(const QModelIndex &parent) const;

signals:

public slots:
     void insert(int row, const QBsonObject &bsonObject);
     void remove(int row, int count = 1);
     void move(int sourceRow, int destinationRow, int count = 1);
     QBsonObject get(int row);
     void reset();

private:

     QBsonItemModel *m_bsonModel;
     QEjdbAbstractSync *m_sync;

     void initCollectionSyncModel(QEjdbDatabase db, const QString &collection);
     bool isValid() const;
     void resetModel();


};

#endif // QEJDBITEMMODEL_H
