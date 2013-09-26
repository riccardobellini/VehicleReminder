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

#ifndef PROFILEVIEW_H
#define PROFILEVIEW_H

// Qt includes
#include <qabstractitemview.h>


class ProfileView : public QAbstractItemView
{
public:
    ProfileView();
    
    virtual QModelIndex indexAt(const QPoint & point);
    virtual void scrollTo(const QModelIndex & index, QAbstractItemView::ScrollHint hint);
    virtual QRect visualRect(const QModelIndex & index);

protected:
    virtual QRegion visualRegionForSelection(const QItemSelection & selection);
    virtual void setSelection(const QRect & rect, QItemSelectionModel::SelectionFlags command);
    virtual bool isIndexHidden(const QModelIndex & index);
    virtual int verticalOffset();
    virtual int horizontalOffset();
    virtual QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers);

};

#endif // PROFILEVIEW_H
