/*
    Copyright (C) 2013  Riccardo Bellini <riccardo.bellini1988@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#ifndef ADDPROFILEDIALOG_H
#define ADDPROFILEDIALOG_H

// KDE includes
#include <kdialog.h>

// Vehicle Reminder includes
#include "profile.h"


class AddProfileWidget;

namespace Ui
{
    class AddProfile;
}

class AddProfileDialog : public KDialog
{
public:
    explicit AddProfileDialog(QWidget *parent = 0);
    
    structures::Profile getProfile() const;
    
    void reset();
    
private:
    AddProfileWidget *m_widget;
};


class AddProfileWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AddProfileWidget(QWidget *parent = 0);
    
    void resetFields();
    
    structures::Profile constructProfile() const;
    
    ~AddProfileWidget();
    
public Q_SLOTS:
    void loadPicture();
    
private:
    Ui::AddProfile *ui;
};

#endif // ADDPROFILEDIALOG_H
