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

// KDE includes
#include <kdebug.h>

// Qt includes
#include <qabstractitemmodel.h>
#include <qsqltablemodel.h>
#include <qsqlerror.h>
#include <qdatawidgetmapper.h>
#include <qstyleditemdelegate.h>

// Vehicle Reminder includes
#include "profilemanagerwidget.h"
#include "profileproxymodel.h"
#include "vrdatabase.h"
#include "constants.h"

// Uis includes
#include "ui_profilemanagerwidget.h"


ProfileManagerWidget::ProfileManagerWidget(QAbstractItemModel * originalModel, QWidget * parent) : QWidget(parent),
    ui(new Ui::ProfileManager), m_originalModel(originalModel), m_proxyModel(0)
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
    
    ui->profileView->setViewMode(QListView::IconMode);
    ui->profileView->setIconSize(ProfilePicturePreviewSize);
    ui->profileView->setFlow(QListView::LeftToRight);
    ui->profileView->setGridSize(ProfileViewGridSize);
    
    ui->applyChangesPushButton->setIcon(KIcon("dialog-ok-apply"));
    // disable the apply changes button initially
    ui->applyChangesPushButton->setEnabled(false);
    // and enable it if something changes
    connect(ui->firstNameLineEdit, SIGNAL(textEdited(QString)), SLOT(m_checkEnableApplyChangesButton()));
    connect(ui->lastNameLineEdit, SIGNAL(textEdited(QString)), SLOT(m_checkEnableApplyChangesButton()));
    connect(ui->birthDateEdit, SIGNAL(dateChanged(QDate)), SLOT(m_checkEnableApplyChangesButton()));
    connect(ui->ssnLineEdit, SIGNAL(textEdited(QString)), SLOT(m_checkEnableApplyChangesButton()));
    connect(ui->licenseNumberLineEdit, SIGNAL(textEdited(QString)), SLOT(m_checkEnableApplyChangesButton()));
    connect(ui->issuingDateEdit, SIGNAL(dateChanged(QDate)), SLOT(m_checkEnableApplyChangesButton()));
    connect(ui->expirationDateEdit, SIGNAL(dateChanged(QDate)), SLOT(m_checkEnableApplyChangesButton()));
    connect(ui->validityYearsNumInput, SIGNAL(valueChanged(int)), SLOT(m_checkEnableApplyChangesButton()));
    connect(ui->notifyCheckBox, SIGNAL(toggled(bool)), SLOT(m_checkEnableApplyChangesButton()));
    connect(ui->otherNotesTextEdit, SIGNAL(textChanged()), SLOT(m_checkEnableApplyChangesButton()));
    
    connect(ui->applyChangesPushButton, SIGNAL(pressed()), SLOT(m_applyChanges()));
    
    // setup mappings with widgets
    m_setupDataMapper();
}


void ProfileManagerWidget::setProxyModel(ProfileProxyModel* model)
{
    if (model) {
        ui->profileView->setModel(model);
        m_proxyModel = model;
        // connect signal to update data mapper
        connect(ui->profileView->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
                this, SLOT(m_updateDataMapperIndex(QItemSelection, QItemSelection)));
        // load data into the mapper, if present
        if (m_proxyModel->rowCount() > 0) {
            ui->profileView->setCurrentIndex(m_proxyModel->index(0, 0, QModelIndex()));
            m_dataMapper->setCurrentIndex(ui->profileView->currentIndex().row());
            ui->applyChangesPushButton->setEnabled(false);
        }
    }
}


// private slots
void ProfileManagerWidget::m_updateDataMapperIndex(const QItemSelection & selected, const QItemSelection & deselected)
{
    if (selected.indexes().isEmpty()) {
        return;
    }
    m_dataMapper->setCurrentIndex(selected.indexes().first().row());
    ui->applyChangesPushButton->setEnabled(false);
}


void ProfileManagerWidget::m_applyChanges()
{
    // let the mapper submit data
    bool submitResult = m_dataMapper->submit();
    if (!submitResult) {
        QSqlTableModel *model = qobject_cast<QSqlTableModel *>(m_originalModel);
        kError() << model->lastError().text();
    }
    // FIXME
    m_dataMapper->setCurrentIndex(0);
}


void ProfileManagerWidget::m_checkEnableApplyChangesButton()
{
    // do not enable apply changes button if either first/last name or license number line edits
    // have empty text
    bool toBeEnabled = !ui->firstNameLineEdit->text().isEmpty() && !ui->lastNameLineEdit->text().isEmpty() &&
        !ui->licenseNumberLineEdit->text().isEmpty();
    // show some nice placeholder text when no text is present
    if (!ui->firstNameLineEdit->text().isEmpty()) {
        ui->firstNameLineEdit->setPlaceholderText(i18n("Enter your first name..."));
    }
    if (!ui->lastNameLineEdit->text().isEmpty()) {
        ui->lastNameLineEdit->setPlaceholderText(i18n("Enter your last name..."));
    }
    if (!ui->licenseNumberLineEdit->text().isEmpty()) {
        ui->licenseNumberLineEdit->setPlaceholderText(i18n("Enter your license number..."));
    }
    ui->applyChangesPushButton->setEnabled(toBeEnabled);
}


// private methods
void ProfileManagerWidget::m_setupDataMapper()
{
    m_dataMapper = new QDataWidgetMapper(this);
    m_dataMapper->setOrientation(Qt::Horizontal);
    m_dataMapper->setModel(m_originalModel);
    // modify submit behaviour of the data mapper
    m_dataMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    // no need for a relational delegate
    m_dataMapper->setItemDelegate(new QStyledItemDelegate(m_dataMapper));
    m_dataMapper->addMapping(ui->firstNameLineEdit, layouts::profile::FirstName);
    m_dataMapper->addMapping(ui->lastNameLineEdit, layouts::profile::LastName);
    m_dataMapper->addMapping(ui->birthDateEdit, layouts::profile::BirthDate);
    m_dataMapper->addMapping(ui->ssnLineEdit, layouts::profile::Ssn);
    m_dataMapper->addMapping(ui->licenseNumberLineEdit, layouts::profile::LicenseNumber);
    m_dataMapper->addMapping(ui->issuingDateEdit, layouts::profile::IssuingDate);
    m_dataMapper->addMapping(ui->expirationDateEdit, layouts::profile::LicenseExpiry);
    m_dataMapper->addMapping(ui->validityYearsNumInput, layouts::profile::LicenseValidityYears);
    m_dataMapper->addMapping(ui->notifyCheckBox, layouts::profile::Notify);
    m_dataMapper->addMapping(ui->otherNotesTextEdit, layouts::profile::OtherNotes);
}
