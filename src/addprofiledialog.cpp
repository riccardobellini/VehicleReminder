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
#include <kfiledialog.h>
#include <kmessagewidget.h>
#include <kdebug.h>

// Qt includes
#include <QDateEdit>
#include <qtimer.h>

// Vehicle Reminder includes
#include "addprofiledialog.h"

// Uis includes
#include "ui_addprofilewidget.h"


using structures::Profile;

AddProfileDialog::AddProfileDialog(QWidget* parent): KDialog(parent)
{
    m_widget = new AddProfileWidget;
    
    setCaption(i18n("Add profile"));
    setButtons(KDialog::Ok | KDialog::Cancel);
    
    setMainWidget(m_widget);
    
    connect(m_widget, SIGNAL(enableOk(bool)), SLOT(enableButtonOk(bool)));
    
    // disable initially the ok button
    enableButtonOk(false);
    
    // reset the fields
    m_widget->resetFields();
}


structures::Profile AddProfileDialog::getProfile() const
{
    return m_widget->constructProfile();
}


void AddProfileDialog::reset()
{
    m_widget->resetFields();
    // disable the ok button
    enableButtonOk(false);
}


// public slots
void AddProfileDialog::accept()
{
    // check correctness of dates
    if (!m_widget->checkFields()) {
        kDebug() << "False check fields";
        return;
    }
    
    KDialog::accept();
}


AddProfileWidget::AddProfileWidget(QWidget *parent): QWidget(parent), ui(new Ui::AddProfile)
{
    ui->setupUi(this);
    
    // setup fields and ranges
    QDate minDate(1900, 1, 1);
    QDate maxDate = QDate::currentDate();
    
    // FIXME try to use KDateComboBox rather than QDateEdit
    ui->birthDateEdit->setDisplayFormat("dd/MM/yyyy");
    ui->birthDateEdit->setMinimumDate(minDate);
    ui->birthDateEdit->setMaximumDate(maxDate);
    ui->birthDateEdit->setDate(maxDate.addYears(-18));
    
    // FIXME try to use KDateComboBox rather than QDateEdit
    ui->issuingDateEdit->setDisplayFormat("dd/MM/yyyy");
    ui->issuingDateEdit->setMinimumDate(minDate);
    ui->issuingDateEdit->setMaximumDate(maxDate);
    ui->issuingDateEdit->setDate(minDate);
    
    // FIXME try to use KDateComboBox rather than QDateEdit
    ui->expirationDateEdit->setDisplayFormat("dd/MM/yyyy");
    ui->expirationDateEdit->setMinimumDate(minDate);
    ui->expirationDateEdit->setMaximumDate(maxDate.addYears(20));
    ui->expirationDateEdit->setDate(maxDate);
    
    ui->validityYearsNumInput->setRange(0, 20);
    
    ui->notifyCheckBox->setChecked(false);
    
    // insert message widget
    m_messageWidget = new KMessageWidget;
    m_messageWidget->setCloseButtonVisible(true);
    m_messageWidget->setMessageType(KMessageWidget::Warning);
    m_messageWidget->hide();
    
    QGridLayout *mainLayout = qobject_cast<QGridLayout *>(layout());
    mainLayout->addWidget(m_messageWidget, 1, 0, 1, 2);
    
    m_messageWidgetHidingTimer = new QTimer(this);
    m_messageWidgetHidingTimer->setSingleShot(true);
    m_messageWidgetHidingTimer->setInterval(4000);
    connect(m_messageWidgetHidingTimer, SIGNAL(timeout()), m_messageWidget, SLOT(animatedHide()));
    
    // connections
    connect(ui->picturePushButton, SIGNAL(pressed()), this, SLOT(loadPicture()));
    
    connect(ui->firstNameLineEdit, SIGNAL(textEdited(QString)), SLOT(m_changed()));
    connect(ui->lastNameLineEdit, SIGNAL(textEdited(QString)), SLOT(m_changed()));
    connect(ui->licenseNumberLineEdit, SIGNAL(textEdited(QString)), SLOT(m_changed()));
}


void AddProfileWidget::resetFields()
{
    // clear all input fields
    ui->firstNameLineEdit->clear();
    ui->lastNameLineEdit->clear();
    ui->birthDateEdit->setDate(QDate::currentDate());
    ui->ssnLineEdit->clear();
    ui->licenseNumberLineEdit->clear();
    ui->issuingDateEdit->setDate(QDate::currentDate());
    ui->expirationDateEdit->setDate(QDate::currentDate());
    ui->validityYearsNumInput->setValue(0);
    ui->notifyCheckBox->setChecked(false);
    ui->picturePushButton->setText(i18n("Choose..."));
    ui->otherNotesTextEdit->clear();
    
    // reset profile picture
    m_currentProfilePicture = QPixmap();
    
    ui->firstNameLineEdit->setPlaceholderText(i18n("Enter your first name..."));
    ui->lastNameLineEdit->setPlaceholderText(i18n("Enter your last name..."));
    ui->licenseNumberLineEdit->setPlaceholderText(i18n("Enter your license number..."));
}


bool AddProfileWidget::checkFields()
{
    kDebug() << ui->expirationDateEdit->date();
    if (ui->expirationDateEdit->date() < QDate::currentDate()) {
        m_messageWidget->setText(i18n("Expiration date must not precede current date"));
        m_messageWidget->animatedShow();
        // start the timer
        m_messageWidgetHidingTimer->start();
        return false;
    }
    
    return true;
}


Profile AddProfileWidget::constructProfile() const
{
    Profile result;
    
    result.firstName = ui->firstNameLineEdit->text();
    result.lastName = ui->lastNameLineEdit->text();
    result.birthDate = ui->birthDateEdit->date();
    result.ssn = ui->ssnLineEdit->text();
    // set picture only if there is a pixmap set
    if (!m_currentProfilePicture.isNull()) {
        result.picture = m_currentProfilePicture;
    }
    result.licenseNumber = ui->licenseNumberLineEdit->text();
    result.issuingDate = ui->issuingDateEdit->date();
    result.licenseExpiryDate = ui->expirationDateEdit->date();
    result.licenseValidityYears = ui->validityYearsNumInput->value();
    result.otherNotes = ui->otherNotesTextEdit->toPlainText();
    result.notify = ui->notifyCheckBox->isChecked();
    
    return result;
}


void AddProfileWidget::loadPicture()
{
    QString imagePath = KFileDialog::getOpenFileName(KUrl(QDir::homePath()), "image/png image/jpeg image/bmp image/gif", this);
    
    if (imagePath.isEmpty()) {
        return;
    }
    
    QPixmap profilePicture(imagePath);
    // save picture at original resolution
    m_currentProfilePicture = profilePicture;
    // display the picture in the label
    QSize labelSize = ui->picturePreviewLabel->size();
    // scale fast, smooth scaling not needed
    profilePicture = profilePicture.scaled(labelSize, Qt::KeepAspectRatio, Qt::FastTransformation);
    ui->picturePreviewLabel->setPixmap(profilePicture);
    
    // modify the text of the button
    ui->picturePushButton->setText(i18n("Change..."));
}


// private slots
void AddProfileWidget::m_changed()
{
    bool toBeEnabled = !ui->firstNameLineEdit->text().isEmpty() && !ui->lastNameLineEdit->text().isEmpty() &&
        !ui->licenseNumberLineEdit->text().isEmpty();
    emit enableOk(toBeEnabled);
    
    if (!ui->firstNameLineEdit->text().isEmpty()) {
        ui->firstNameLineEdit->setPlaceholderText(i18n("Enter your first name..."));
    }
    if (!ui->lastNameLineEdit->text().isEmpty()) {
        ui->lastNameLineEdit->setPlaceholderText(i18n("Enter your last name..."));
    }
    if (!ui->licenseNumberLineEdit->text().isEmpty()) {
        ui->licenseNumberLineEdit->setPlaceholderText(i18n("Enter your license number..."));
    }
}


AddProfileWidget::~AddProfileWidget()
{
    delete ui;
}
