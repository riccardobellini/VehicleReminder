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


#ifndef ADDPROFILEDIALOG_H
#define ADDPROFILEDIALOG_H

// KDE includes
#include <kdialog.h>

// Vehicle Reminder includes
#include "profile.h"


class AddProfileWidget;
class QTimer;
class KMessageWidget;

namespace Ui
{
    class AddProfile;
}

class AddProfileDialog : public KDialog
{
    Q_OBJECT
public:
    explicit AddProfileDialog(QWidget *parent = 0);
    
    structures::Profile getProfile() const;
    
    void reset();
    
public Q_SLOTS:
    virtual void accept();
    
private:
    AddProfileWidget *m_widget;
};


class AddProfileWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AddProfileWidget(QWidget *parent = 0);
    
    void resetFields();
    bool checkFields();
    
    structures::Profile constructProfile() const;
    
    ~AddProfileWidget();
    
Q_SIGNALS:
    void enableOk(bool enable);
    
public Q_SLOTS:
    void loadPicture();
    
private Q_SLOTS:
    void m_changed();
    
private:
    Ui::AddProfile *ui;
    
    QPixmap m_currentProfilePicture;
    
    KMessageWidget *m_messageWidget;
    QTimer *m_messageWidgetHidingTimer;
};

#endif // ADDPROFILEDIALOG_H
