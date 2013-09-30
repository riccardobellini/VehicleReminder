/*
 * Copyright (C) 2013  Riccardo Bellini <riccardo.belini1988@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef PROFILEPROXYMODEL_H
#define PROFILEPROXYMODEL_H

// Qt includes
#include <qabstractproxymodel.h>


class ProfileProxyModel : public QAbstractProxyModel
{
public:
    explicit ProfileProxyModel(QObject * parent = 0);
    
    virtual QModelIndex mapFromSource(const QModelIndex& sourceIndex);
    virtual QModelIndex mapToSource(const QModelIndex& proxyIndex);
    virtual QVariant data(const QModelIndex & proxyIndex, int role = Qt::DisplayRole) const;
    virtual int columnCount(const QModelIndex& parent);
    virtual int rowCount(const QModelIndex& parent);
    virtual QModelIndex parent(const QModelIndex& child);
    virtual QModelIndex index(int row, int column, const QModelIndex& parent);
};

#endif // PROFILEPROXYMODEL_H
