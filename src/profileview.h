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
    Q_OBJECT
public:
    ProfileView(QWidget * parent = 0);
    
    virtual QModelIndex indexAt(const QPoint & point);
    virtual void scrollTo(const QModelIndex & index, QAbstractItemView::ScrollHint hint);
    virtual void setModel(QAbstractItemModel * model);
    virtual QRect visualRect(const QModelIndex & index);
    
    static const QSize ProfilePictureSize;
    static const int MaxTextWidth;
    static const int SpacingWidth;
    static const int SpacingHeight;
    static const int SpacingPictureText;
    
protected slots:
    void dataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight);
    void rowsInserted(const QModelIndex & parent, int start, int end);
    void rowsAboutToBeRemoved(const QModelIndex & parent, int start, int end);

protected:
    virtual QRegion visualRegionForSelection(const QItemSelection & selection);
    virtual void setSelection(const QRect & rect, QItemSelectionModel::SelectionFlags command);
    virtual bool isIndexHidden(const QModelIndex & index);
    virtual int verticalOffset();
    virtual int horizontalOffset();
    virtual QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers);
    virtual void scrollContentsBy(int dx, int dy);
    virtual void paintEvent(QPaintEvent * event);

private:
    void m_calculateRects() const;
    QRectF m_viewportRectForRow(int row) const;
    
    mutable QHash<int, QRectF> m_rectForRow;
    mutable bool m_hashIsDirty;
    mutable int idealHeight;
};

#endif // PROFILEVIEW_H
