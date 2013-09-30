/*
 * <one line to give the program's name and a brief idea of what it does.>
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

// Vehicle Reminder includes
#include "profileproxymodel.h"


ProfileProxyModel::ProfileProxyModel(QObject * parent) : QAbstractProxyModel(parent)
{

}


QModelIndex ProfileProxyModel::mapFromSource(const QModelIndex & sourceIndex)
{
    // since column count is equal to 1, we use just the row number
    return index(sourceIndex.row(), 0, QModelIndex());
}


QModelIndex ProfileProxyModel::mapToSource(const QModelIndex & proxyIndex)
{
    QModelIndex index;
    
    // TODO
    
    return index;
}


QVariant ProfileProxyModel::data(const QModelIndex & proxyIndex, int role) const
{
    QVariant result;
    
    // TODO
    
    return result;
}


int ProfileProxyModel::columnCount(const QModelIndex & parent)
{
    int result;
    
    // TODO
    
    return result;
}


int ProfileProxyModel::rowCount(const QModelIndex & parent)
{
    int result;
    
    // TODO
    
    return result;
}


QModelIndex ProfileProxyModel::parent(const QModelIndex & child)
{
    QModelIndex index;
    
    // TODO
    
    return index;
}


QModelIndex ProfileProxyModel::index(int row, int column, const QModelIndex & parent)
{
    QModelIndex index;
    
    // TODO
    
    return index;
}
