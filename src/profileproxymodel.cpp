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
#include "constants.h"

// Qt includes
#include <qfontmetrics.h>

// KDE includes
#include <kapplication.h>
#include <kdebug.h>


ProfileProxyModel::ProfileProxyModel(QObject * parent) : QAbstractProxyModel(parent)
{
    
}


void ProfileProxyModel::setSourceModel(QAbstractItemModel* sourceModel)
{
    // call the base class implementation
    QAbstractProxyModel::setSourceModel(sourceModel);
    // WARNING model must not be null
    // connect dataChanged signal to private slot
    connect(sourceModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)), this,
            SLOT(m_modelDataChanged(const QModelIndex &, const QModelIndex &)));
    // connect rowsInserted signal to private slot
    connect(sourceModel, SIGNAL(rowsInserted(const QModelIndex &, int, int)), this,
            SLOT(m_modelRowInserted(const QModelIndex &, int, int)));
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
        QFontMetrics fontMetrics = kapp->fontMetrics();
        if (fontMetrics.width(result) > MaxProfileNameWidth) {
            result = fontMetrics.elidedText(result, Qt::ElideMiddle, MaxProfileNameWidth);
        }
        return result;
    }
    if (role == Qt::DecorationRole || role == PictureRole) {
        QModelIndex pictureIndex = sourceIndex.sibling(sourceIndex.row(), layouts::profile::Picture);
        QByteArray byteArray = pictureIndex.data().toByteArray();
        QImage image;
        if (!image.loadFromData(byteArray)) {
            return QVariant();
        }
        if (role == Qt::DecorationRole) {
            // scale the image
            image = image.scaledToHeight(ProfilePicturePreviewSize.height(), Qt::FastTransformation);
        }
        QVariant variant = image;
        return variant;
    }
    
    return QVariant();
}


Qt::ItemFlags ProfileProxyModel::flags(const QModelIndex& proxyIndex) const
{
    // item is not editable
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


int ProfileProxyModel::columnCount(const QModelIndex & parent) const
{
    // there is just one column
    Q_UNUSED(parent);
    return 1;
}


int ProfileProxyModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return sourceModel()->rowCount();
}


bool ProfileProxyModel::canFetchMore(const QModelIndex& parent) const
{
    return sourceModel()->canFetchMore(mapToSource(parent));
}


void ProfileProxyModel::fetchMore(const QModelIndex& parent)
{
    sourceModel()->fetchMore(mapToSource(parent));
}


QModelIndex ProfileProxyModel::parent(const QModelIndex & child) const
{
    // flat model, no index has a valid parent
    return QModelIndex();
}


QModelIndex ProfileProxyModel::index(int row, int column, const QModelIndex & parent) const
{
    // WARNING might be wrong
    return createIndex(row, column);
}



// private slots
void ProfileProxyModel::m_modelDataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight)
{
    kDebug() << "Data changed in indexes " << topLeft << " " << bottomRight;
    emit dataChanged(mapFromSource(topLeft), mapFromSource(bottomRight));
}


void ProfileProxyModel::m_modelRowInserted(const QModelIndex& parent, int start, int end)
{
    // insert row
    QModelIndex proxyParent = mapFromSource(parent);
    beginInsertRows(proxyParent, start, end);
    endInsertRows();
}
