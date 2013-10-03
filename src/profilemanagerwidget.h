/*
 * Copyright (C) 2013  Riccardo Bellini <riccardo.bellini1988@gmail.com>
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

#ifndef PROFILEMANAGERWIDGET_H
#define PROFILEMANAGERWIDGET_H

// Qt includes
#include <qwidget.h>


namespace Ui
{
    class ProfileManager;
}
class QAbstractItemModel;
class ProfileProxyModel;
class QItemSelection;
class QDataWidgetMapper;

class ProfileManagerWidget : public QWidget
{
    Q_OBJECT
public:
    ProfileManagerWidget(QAbstractItemModel * originalModel, QWidget * parent = 0);
    
    void setProxyModel(ProfileProxyModel * model);

private slots:
    void m_updateDataMapperIndex(const QItemSelection & selected, const QItemSelection & deselected);
    
private:
    void m_setupDataMapper();
    
    Ui::ProfileManager *ui;
    
    QAbstractItemModel *m_originalModel;
    ProfileProxyModel *m_proxyModel;
    
    QDataWidgetMapper *m_dataMapper;
};

#endif // PROFILEMANAGERWIDGET_H
