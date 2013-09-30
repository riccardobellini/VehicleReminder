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
#include "vrdatabase.h"


ProfileProxyModel::ProfileProxyModel(QObject * parent) : QAbstractProxyModel(parent)
{

}


QModelIndex ProfileProxyModel::mapFromSource(const QModelIndex & sourceIndex) const
{
    // since column count is equal to 1, we use just the row number
    return index(sourceIndex.row(), 0, QModelIndex());
}


QModelIndex ProfileProxyModel::mapToSource(const QModelIndex & proxyIndex) const
{
    // since column count is equal to 1, map to the "id" column of the source
    QModelIndex index = sourceModel()->index(proxyIndex.row(), layouts::profile::Id, QModelIndex());
    return index;
}


QVariant ProfileProxyModel::data(const QModelIndex & proxyIndex, int role) const
{
    // check if the proxyIndex is valid
    if (!proxyIndex.isValid()) {
        return QVariant();
    }
    QModelIndex sourceIndex = mapToSource(proxyIndex);
    // check for the role
    if (role == Qt::DisplayRole) {
        QModelIndex firstNameIndex = sourceIndex.sibling(sourceIndex.row(), layouts::profile::FirstName);
        QModelIndex lastNameIndex = sourceIndex.sibling(sourceIndex.row(), layouts::profile::LastName);
        QString firstName = firstNameIndex.data().toString();
        QString lastName = lastNameIndex.data().toString();
        // return firstName and lastName combined
        QString result = firstName + " " + lastName;
        return result;
    }
    if (role == PictureRole) {
        QModelIndex pictureIndex = sourceIndex.sibling(sourceIndex.row(), layouts::profile::Picture);
        QByteArray byteArray = pictureIndex.data().toByteArray();
        // return the byte array, it will be up to the delegate to display the preview
        return byteArray;
    }
    
    return QVariant();
}


int ProfileProxyModel::columnCount(const QModelIndex & parent)
{
    // there is just one column
    Q_UNUSED(parent);
    return 1;
}


int ProfileProxyModel::rowCount(const QModelIndex & parent)
{
    Q_UNUSED(parent);
    return sourceModel()->rowCount();
}


QModelIndex ProfileProxyModel::parent(const QModelIndex & child)
{
    // flat model, no index has a valid parent
    return QModelIndex();
}


QModelIndex ProfileProxyModel::index(int row, int column, const QModelIndex & parent) const
{
    QModelIndex index;
    
    // TODO
    
    return index;
}
