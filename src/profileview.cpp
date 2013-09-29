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
#include <qpainter.h>

// Vehicle Reminder includes
#include "profileview.h"
#include "vrdatabase.h"


const QSize ProfileView::ProfilePictureSize = QSize(64, 64);
const int ProfileView::MaxTextWidth = 120;
const int ProfileView::SpacingWidth = 30;
const int ProfileView::SpacingHeight = 30;
const int ProfileView::SpacingPictureText = 10;

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
    QPoint copyPoint(point);
    copyPoint.rx() += horizontalScrollBar()->value(); // should be 0
    copyPoint.ry() += verticalScrollBar()->value();
    m_calculateRects();
    // NOTE iteration could probably be a bottleneck if thousands of elements are in the model,
    // but we assume that the number of profile is never this high
    QHashIterator<int, QRectF> it(m_rectForRow);
    while (it.hasNext()) {
        it.next();
        if (it.value().contains(copyPoint)) {
            return model()->index(it.key(), layouts::profile::Id, rootIndex());
        }
    }
    return QModelIndex();
}


void ProfileView::scrollTo(const QModelIndex & index, QAbstractItemView::ScrollHint hint)
{
    QRect viewRect = viewport()->rect();
    QRect itemRect = visualRect(index);
    
    // no check of left and right edges of the itemRect
    // to adjust the horizontal scroll bar
    if (itemRect.top() < viewRect.top()) {
        verticalScrollBar()->setValue(verticalScrollBar()->value() + 
            itemRect.top() - viewRect.top());
    }
    else if (itemRect.bottom() > viewRect.bottom()) {
        verticalScrollBar()->setValue(verticalScrollBar()->value() + 
            qMin(itemRect.bottom() - viewRect.bottom(),
                itemRect.top() - viewRect.top()));
    }
    viewport()->update();
}


void ProfileView::setModel(QAbstractItemModel* model)
{
    QAbstractItemView::setModel(model);
    m_hashIsDirty = true;
}


QRect ProfileView::visualRect(const QModelIndex & index)
{
    QRect rect;
    if (index.isValid()) {
        rect = m_viewportRectForRow(index.row()).toRect();
    }
    return rect;
}


// protected slots
void ProfileView::dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    m_hashIsDirty = true;
    QAbstractItemView::dataChanged(topLeft, bottomRight);
}


void ProfileView::rowsInserted(const QModelIndex& parent, int start, int end)
{
    m_hashIsDirty = true;
    QAbstractItemView::rowsInserted(parent, start, end);
}


void ProfileView::rowsAboutToBeRemoved(const QModelIndex& parent, int start, int end)
{
    m_hashIsDirty = true;
    QAbstractItemView::rowsAboutToBeRemoved(parent, start, end);
}


// protected methods
QRegion ProfileView::visualRegionForSelection(const QItemSelection & selection)
{
    QRegion region;
    foreach (const QItemSelectionRange & range, selection) {
        for (int row = range.top(); row <= range.bottom(); ++row) {
            for (int column = range.left(); column <= range.right(); ++column) {
                QModelIndex index = model()->index(row, column, rootIndex());
                region += visualRect(index);
            }
        }
    }
    return region;
}


void ProfileView::setSelection(const QRect & rect, QItemSelectionModel::SelectionFlags command)
{
    QRect rectangle = rect.translated(horizontalScrollBar()->value(),
        verticalScrollBar()->value()).normalized();
    m_calculateRects();
    // NOTE iteration could probably be a bottleneck if thousands of elements are in the model,
    // but we assume that the number of profile is never this high
    QHashIterator<int, QRectF> it(m_rectForRow);
    int firstRow = model()->rowCount();
    int lastRow = -1;
    while (it.hasNext()) {
        it.next();
        if (it.value().intersects(rectangle)) {
            firstRow = firstRow < it.key() ? firstRow : it.key();
            lastRow = lastRow > it.key() ? lastRow : it.key();
        }
        if (firstRow != model()->rowCount() && lastRow != -1) {
            QItemSelection selection(model()->index(firstRow, layouts::profile::Id, rootIndex()),
                model()->index(lastRow, layouts::profile::Id, rootIndex()));
            selectionModel()->select(selection, command);
        }
        else {
            QModelIndex invalid;
            QItemSelection selection(invalid, invalid);
            selectionModel()->select(selection, command);
        }
    }
}


bool ProfileView::isIndexHidden(const QModelIndex & index)
{
    // WARNING check this return
    return false;
}


int ProfileView::verticalOffset()
{
    return verticalScrollBar()->value();
}


int ProfileView::horizontalOffset()
{
    return horizontalScrollBar()->value();
}


QModelIndex ProfileView::moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers)
{
    QModelIndex index = currentIndex();
    if (index.isValid()) {
        if ((cursorAction == MoveLeft && index.row()) > 0 ||
            (cursorAction == MoveRight && index.row() + 1 < model()->rowCount())) {
                const int Offset = (cursorAction == MoveLeft ? -1 : 1);
                index = model()->index(index.row() + Offset, index.column(), index.parent());
        }
        else if ((cursorAction == MoveUp && index.row()) > 0 ||
                 (cursorAction == MoveDown && index.row() + 1 < model()->rowCount())) {
            QFontMetrics fontMetrics(font());
            const int RowHeight = (ProfilePictureSize.height() + SpacingPictureText + fontMetrics.height()) *
                (cursorAction == MoveUp ? -1 : 1);
            QRect rect = m_viewportRectForRow(index.row()).toRect();
            QPoint point(rect.center().x(), rect.center().y() + RowHeight);
            while (point.x() >= 0) {
                index = indexAt(point);
                if (index.isValid()) {
                    break;
                }
                point.rx() -= fontMetrics.width("n");
            }
        }
    }
    
    return index;
}


void ProfileView::scrollContentsBy(int dx, int dy)
{
    scrollDirtyRegion(dx, dy);
    viewport()->scroll(dx, dy);
}


void ProfileView::paintEvent(QPaintEvent * event)
{
    Q_UNUSED(event);
    QPainter painter(viewport());
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    for (int row = 0; row < model()->rowCount(rootIndex()); ++row) {
        QModelIndex index = model()->index(row, layouts::profile::Id, rootIndex());
        QRectF rect = m_viewportRectForRow(row);
        if (!rect.isValid() || rect.bottom() < 0 || rect.y() > viewport()->height()) {
            continue;
        }
        QStyleOptionViewItem option = viewOptions();
        option.rect = rect.toRect();
        if (selectionModel()->isSelected(index)) {
            option.state |= QStyle::State_Selected;
        }
        if (currentIndex() == index) {
            option.state != QStyle::State_HasFocus;
        }
        itemDelegate()->paint(&painter, option, index);
    }
}


// private methods
void ProfileView::m_calculateRects() const
{
    if (!m_hashIsDirty) {
        return;
    }
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
            QModelIndex firstNameIndex = model()->index(row, layouts::profile::FirstName, rootIndex());
            QModelIndex lastNameIndex = model()->index(row, layouts::profile::LastName, rootIndex());
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


QRectF ProfileView::m_viewportRectForRow(int row) const
{
    // calculate rects, if necessary
    m_calculateRects();
    QRectF rect = m_rectForRow.value(row).toRect();
    if (!rect.isValid()) {
        return rect;
    }
    return QRectF(rect.x(), rect.y() - verticalScrollBar()->value(),
        rect.width(), rect.height());
}
