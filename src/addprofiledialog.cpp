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

// Vehicle Reminder includes
#include "addprofiledialog.h"

// Uis includes
#include "ui_addprofilewidget.h"


AddProfileDialog::AddProfileDialog(QWidget* parent): KDialog(parent)
{
    m_widget = new AddProfileWidget;
    
    setCaption(i18n("Add profile"));
    setButtons(KDialog::Ok | KDialog::Cancel);
        
    setMainWidget(m_widget);
}


void AddProfileDialog::reset()
{
    m_widget->resetFields();
}


AddProfileWidget::AddProfileWidget(QWidget *parent): QWidget(parent), ui(new Ui::AddProfile)
{
    ui->setupUi(this);
}


void AddProfileWidget::resetFields()
{
    // TODO
}


AddProfileWidget::~AddProfileWidget()
{
    delete ui;
}