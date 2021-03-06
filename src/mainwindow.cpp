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
#include <kaction.h>
#include <kactioncollection.h>
#include <kapplication.h>
#include <ktoolbar.h>
#include <kmenubar.h>
#include <kmessagebox.h>
#include <ksharedconfig.h>
#include <kconfiggroup.h>
#include <kiconloader.h>
#include <kdebug.h>

// Qt includes
#include <qscrollarea.h>
#include <QHBoxLayout>
#include <qsplitter.h>
#include <qstackedwidget.h>
#include <qsqltablemodel.h>
#include <qsqlrecord.h>
#include <qbuffer.h>
#include <qimagewriter.h>

// Vehicle Reminder includes
#include "mainwindow.h"
#include "vrdatabase.h"
#include "addprofiledialog.h"
#include "profilemanagerwidget.h"
#include "profile.h"
#include "profileproxymodel.h"
#include "constants.h"

// Uis includes
#include "ui_mainwidget.h"

class MainWindowPrivate
{
public:
    Ui::mainWidget ui;
    QHBoxLayout *m_mainLayout;
    QStackedWidget *m_mainStackedWidget;
    QSplitter *m_splitter;
    
    KAction *m_addProfileAction;
    KAction *m_openProfileAction;
    KAction *m_removeProfileAction;
    
    VRDatabase *m_database;
    
    AddProfileDialog *m_addProfileDialog;
    
    ProfileManagerWidget *m_profileManagerWidget;
    QSqlTableModel *m_profileModel;
    ProfileProxyModel *m_profileProxyModel;
};


using structures::Profile;

MainWindow::MainWindow() : d(new MainWindowPrivate)
{
    QScrollArea *area = new QScrollArea(this);
    area->setFrameShape(QFrame::NoFrame);
    area->setFocusPolicy(Qt::NoFocus);
    d->ui.setupUi(area);
    d->m_mainLayout = new QHBoxLayout(area);
    d->m_mainLayout->setSpacing(0);
    d->m_mainLayout->setContentsMargins(0, 0, 0, 0);
    d->m_mainLayout->addWidget(d->ui.dockWidget, 1);
    d->m_mainStackedWidget = new QStackedWidget(area);
    
    d->m_addProfileDialog = 0;

    // FIXME size of widgets
    d->m_splitter = new QSplitter(Qt::Horizontal, area);
    d->m_splitter->addWidget(d->ui.dockWidget);
    d->m_splitter->setStretchFactor(0, 1);
    d->m_splitter->addWidget(d->m_mainStackedWidget);
    d->m_splitter->setStretchFactor(1, 2);
    
    d->m_mainLayout->addWidget(d->m_splitter);
    
    QPalette palette = QApplication::palette();
    palette.setColor(QPalette::Base, Qt::transparent);
    d->ui.klistwidget->setPalette(palette);
    d->ui.klistwidget->setFrameShape(QFrame::NoFrame);
    
    setCentralWidget(area);
    
    m_setupActions();
    
    d->m_database = new VRDatabase(this);
    // open with default path, temporarily
    if (!d->m_database->open()) {
        kFatal() << "Unable to open database, closing";
        KMessageBox::error(this, i18n("Unable to open database, closing"), i18n("Fatal error"));
        kapp->quit();
    }
    
    KConfigGroup generalCfgGroup = KGlobal::config()->group("Database");
    QString latestVersion = d->m_database->LatestVersion;
    QString version = generalCfgGroup.readEntry("version", QString());
    if (version.isEmpty()) {
        // first startup
        bool initSuccessful = d->m_database->init();
        if (initSuccessful) {
            generalCfgGroup.writeEntry("version", latestVersion);
        }
    } else if (version != latestVersion) {
        // migrate
        int result = KMessageBox::questionYesNo(this, i18n("Your database needs to be migrated to the latest version. "
            "Would you like to do the migration now? Answering \"No\" will close the application."), i18n("Database "
            "migration"));
        if (result == KMessageBox::Yes) {
            bool migrationSuccessful = d->m_database->migrate(version, latestVersion);
            if (migrationSuccessful) {
                generalCfgGroup.writeEntry("version", latestVersion);
            }
        } else if (result == KMessageBox::No) {
            kapp->closeAllWindows();
        }
    } else if (version == latestVersion) {
        kDebug() << "Well done, found latest version of the database";
    }
    
    m_setupContentsList();
    
    setupGUI(Default, "mainwindow.rc");
}


void MainWindow::addProfile()
{
    kDebug() << "Add profile called";
    if (!d->m_addProfileDialog) {
        d->m_addProfileDialog = new AddProfileDialog(this);
    } else {
        d->m_addProfileDialog->reset();
    }
    int result = d->m_addProfileDialog->exec();
    if (result == KDialog::Accepted) {
        Profile newProfile = d->m_addProfileDialog->getProfile();
        // add the profile to the database
        QSqlRecord record = d->m_profileModel->record();
        record.setValue(layouts::profile::FirstName, newProfile.firstName);
        record.setValue(layouts::profile::LastName, newProfile.lastName);
        record.setValue(layouts::profile::BirthDate, newProfile.birthDate);
        record.setValue(layouts::profile::Ssn, newProfile.ssn);
        // prepare insertion of picture
        QByteArray byteArray;
        // convert pixmap to image
        QImage profileImage;
        if (newProfile.picture.isNull()) {
            KIconLoader *iconLoader = KIconLoader::global();
            QString userIconPath = iconLoader->iconPath("user-identity.png", KIconLoader::Desktop);
            profileImage = QImage(userIconPath);
        }
        else {
            profileImage = newProfile.picture.toImage();
        }
        // reduce it to a suitable format
        if (profileImage.size().width() > ProfilePictureSize.width() ||
            profileImage.size().height() > ProfilePictureSize.height()) {
            profileImage = profileImage.scaled(ProfilePictureSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }
        QBuffer buffer;
        QImageWriter imageWriter(&buffer, "PNG");
        imageWriter.write(profileImage);
        byteArray.append(buffer.data());
        record.setValue(layouts::profile::Picture, byteArray);
        record.setValue(layouts::profile::LicenseNumber, newProfile.licenseNumber);
        record.setValue(layouts::profile::IssuingDate, newProfile.issuingDate);
        record.setValue(layouts::profile::LicenseExpiry, newProfile.licenseExpiryDate);
        record.setValue(layouts::profile::LicenseValidityYears, newProfile.licenseValidityYears);
        record.setValue(layouts::profile::OtherNotes, newProfile.otherNotes);
        record.setValue(layouts::profile::Notify, newProfile.notify);
        // append the record to the model
        d->m_profileModel->insertRecord(-1, record);
    }
}


void MainWindow::openProfile()
{
    // TODO
}


void MainWindow::removeProfile()
{
    // TODO
}


// private slots
void MainWindow::m_activateWidget()
{
    QModelIndex index = d->ui.klistwidget->selectionModel()->selectedIndexes().first();
    if (index.isValid()) {
        d->m_mainStackedWidget->setCurrentIndex(index.row());
        return;
    }
    kDebug() << "Invalid index selected";
}


void MainWindow::m_setupActions()
{
    d->m_addProfileAction = new KAction(KIcon("list-add-user"), i18n("Add profile"), this);
    actionCollection()->addAction("addProfile", d->m_addProfileAction);
    connect(d->m_addProfileAction, SIGNAL(triggered(Qt::MouseButtons,Qt::KeyboardModifiers)), this, SLOT(addProfile()));
    
    d->m_openProfileAction = new KAction(KIcon("document-open"), i18n("Open profile"), this);
    actionCollection()->addAction("openProfile", d->m_openProfileAction);
    connect(d->m_openProfileAction, SIGNAL(triggered(Qt::MouseButtons,Qt::KeyboardModifiers)), this, SLOT(openProfile()));
    
    d->m_removeProfileAction = new KAction(KIcon("list-remove-user"), i18n("Remove profile"), this);
    actionCollection()->addAction("removeProfile", d->m_removeProfileAction);
    connect(d->m_removeProfileAction, SIGNAL(triggered(Qt::MouseButtons,Qt::KeyboardModifiers)), this, SLOT(removeProfile()));
    
    KAction *quitAction = KStandardAction::quit(kapp, SLOT(closeAllWindows()), this);
    actionCollection()->addAction("quit", quitAction);
}


void MainWindow::m_setupContentsList()
{
    // set properties of list
    d->ui.klistwidget->setSelectionMode(QAbstractItemView::SingleSelection);
    d->ui.klistwidget->setIconSize(ContentsListIconSize);
    
    // add element to the main list
    QListWidgetItem *profileManagerItem = new QListWidgetItem;
    profileManagerItem->setText(i18n("Profile manager"));
    profileManagerItem->setStatusTip(i18n("Click to review and edit profiles"));
    profileManagerItem->setToolTip(i18n("Review and edit profiles"));
    profileManagerItem->setIcon(KIcon("user-identity"));
    d->ui.klistwidget->addItem(profileManagerItem);
    
    // initialize profile model and proxy
    d->m_profileModel = new QSqlTableModel(this, d->m_database->getDatabase());
    d->m_profileModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    d->m_profileModel->setTable("profile");
    d->m_profileModel->select();
    
    d->m_profileProxyModel = new ProfileProxyModel(this);
    d->m_profileProxyModel->setSourceModel(d->m_profileModel);
    
    d->m_profileManagerWidget = new ProfileManagerWidget(d->m_profileModel);
    d->m_profileManagerWidget->setProxyModel(d->m_profileProxyModel);
    d->m_mainStackedWidget->addWidget(d->m_profileManagerWidget);
    d->m_profileManagerWidget->show();

    connect(d->ui.klistwidget, SIGNAL(itemSelectionChanged()), SLOT(m_activateWidget()));
}
