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

// KDE includes
#include <kapplication.h>

// Qt includes
#include <qscrollbar.h>

// Vehicle Reminder includes
#include "profileview.h"


ProfileView::ProfileView(QWidget * parent) : QAbstractItemView(parent), hashIsDirty(false)
{
    setFocusPolicy(Qt::WheelFocus);
    setFont(KApplication::font("QListView"));
    horizontalScrollBar()->setRange(0, 0);
    verticalScrollBar()->setRange(0, 0);
}


QModelIndex ProfileView::indexAt(const QPoint & point)
{
    QModelIndex index;
    
    // TODO
    
    return index;
}


void ProfileView::scrollTo(const QModelIndex & index, QAbstractItemView::ScrollHint hint)
{
    // TODO
}


void ProfileView::setModel(QAbstractItemModel* model)
{
    QAbstractItemView::setModel(model);
    hashIsDirty = true;
}


QRect ProfileView::visualRect(const QModelIndex & index)
{
    QRect rect;
    
    // TODO
    
    return rect;
}


QRegion ProfileView::visualRegionForSelection(const QItemSelection & selection)
{
    QRegion region;
    
    // TODO
    
    return region;
}


void ProfileView::setSelection(const QRect & rect, QItemSelectionModel::SelectionFlags command)
{
    // TODO
}


bool ProfileView::isIndexHidden(const QModelIndex & index)
{
    // TODO
    
    return false;
}


int ProfileView::verticalOffset()
{
    int result;
    
    // TODO
    
    return result;
}


int ProfileView::horizontalOffset()
{
    int result;
    
    // TODO
    
    return result;
}


QModelIndex ProfileView::moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers)
{
    QModelIndex index;
    
    // TODO
    
    return index;
}
