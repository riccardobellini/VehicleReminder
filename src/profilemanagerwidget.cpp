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

// Vehicle Reminder includes
#include "profilemanagerwidget.h"
#include "profileproxymodel.h"

// Uis includes
#include "ui_profilemanagerwidget.h"


ProfileManagerWidget::ProfileManagerWidget(QWidget * parent) : QWidget(parent),
    ui(new Ui::ProfileManager)
{
    ui->setupUi(this);
    
    QDate minDate(1900, 1, 1);
    QDate maxDate(QDate::currentDate());
    
    // FIXME try to use KDateComboBox rather than QDateEdit
    ui->birthDateEdit->setDisplayFormat("dd/MM/yyyy");
    ui->birthDateEdit->setMinimumDate(minDate);
    ui->birthDateEdit->setMaximumDate(maxDate);
    
    // FIXME try to use KDateComboBox rather than QDateEdit
    ui->issuingDateEdit->setDisplayFormat("dd/MM/yyyy");
    ui->issuingDateEdit->setMinimumDate(minDate);
    ui->issuingDateEdit->setMaximumDate(maxDate);
    
    // FIXME try to use KDateComboBox rather than QDateEdit
    ui->expirationDateEdit->setDisplayFormat("dd/MM/yyyy");
    ui->expirationDateEdit->setMinimumDate(maxDate);
    ui->expirationDateEdit->setMaximumDate(maxDate.addYears(20));
    
    ui->validityYearsNumInput->setRange(0, 20);
    
    ui->notifyCheckBox->setChecked(false);
}


void ProfileManagerWidget::initModel(ProfileProxyModel* model)
{
    ui->profileView->setModel(model);
}
