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
#include "vrdatabase.h"


const QSize ProfileView::ProfilePictureSize = QSize(64, 64);
const int ProfileView::MaxTextWidth = 120;

ProfileView::ProfileView(QWidget * parent) : QAbstractItemView(parent), m_hashIsDirty(false),
    idealHeight(0)
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
    m_hashIsDirty = true;
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


// private methods
void ProfileView::m_calculateRects() const
{
    if (!m_hashIsDirty) {
        return;
    }
    const int SpacingWidth = 30;
    const int SpacingHeight = 30;
    const int SpacingPictureText = 10;
    QFontMetrics fontMetrics(font());
    // row height is equal to the height of the picture, plus the spacing, plus
    // the height of the text
    const int RowHeight = ProfilePictureSize.height() + SpacingPictureText + fontMetrics.height();
    const int MaxWidth = viewport()->width();
    int minimumWidth = 0;
    int x = 0;
    int y = 0;
    // use forever and fetchMore() and canFetchMore() since the model is a SQL model
    forever {
        for (int row = 0; row < model()->rowCount(); ++row) {
            QModelIndex firstNameIndex = model()->index(row, VRDatabase::ProfileColumnLayout::FirstName, rootIndex());
            QModelIndex lastNameIndex = model()->index(row, VRDatabase::ProfileColumnLayout::LastName, rootIndex());
            QString text = model()->data(firstNameIndex).toString() + " " + model()->data(lastNameIndex).toString();
            // if text is too long, elide it and display just MaxTextWidth
            int textWidth = fontMetrics.width(text) <= MaxTextWidth ? fontMetrics.width(text) : MaxTextWidth;
            if (!(x == 0 || x + textWidth < MaxWidth)) {
                y += RowHeight + SpacingHeight;
                x = 0;
            }
            else if (x != 0) {
                x += SpacingWidth;
            }
            m_rectForRow[row] = QRectF(x, y, textWidth, RowHeight);
        }
        if (!model()->canFetchMore(rootIndex())) {
            break;
        }
        model()->fetchMore(rootIndex());
    }
    idealHeight = y + RowHeight;
    m_hashIsDirty = false;
    viewport()->update();
}
