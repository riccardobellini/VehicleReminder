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
class QTimer;
class QLabel;
class QEvent;
class KMessageWidget;
class KMenu;
class KAction;

class ProfileManagerWidget : public QWidget
{
    Q_OBJECT
public:
    ProfileManagerWidget(QAbstractItemModel * originalModel, QWidget * parent = 0);
    
    virtual ~ProfileManagerWidget();
    
    void setProxyModel(ProfileProxyModel * model);
    
protected:
    virtual bool eventFilter(QObject * target, QEvent * event);

private slots:
    void m_updateDataMapperIndex(const QItemSelection & selected, const QItemSelection & deselected);
    void m_applyChanges();
    void m_checkEnableApplyChangesButton();
    
    void m_contextMenuRequested(const QPoint & pos);
    
    void m_viewPicture();
    void m_changePicture();
    
private:
    void m_setupDataMapper();
    void m_setupContextMenu();
    
    Ui::ProfileManager *ui;
    
    QAbstractItemModel *m_originalModel;
    ProfileProxyModel *m_proxyModel;
    
    QDataWidgetMapper *m_dataMapper;
    
    KMessageWidget *m_messageWidget;
    QTimer *m_messageWidgetHidingTimer;
    
    // context menu and actions
    KMenu *m_contextMenu;
    KAction *m_changePictureAction;
    KAction *m_viewPictureAction;
    
    QLabel *m_pictureLabel;
};

#endif // PROFILEMANAGERWIDGET_H
