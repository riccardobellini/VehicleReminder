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

// KDE includes
#include <kaction.h>
#include <kactioncollection.h>
#include <kapplication.h>
#include <ktoolbar.h>
#include <kmenubar.h>
#include <kmessagebox.h>
#include <kdebug.h>

// Qt includes
#include <qscrollarea.h>
#include <QHBoxLayout>
#include <qsplitter.h>
#include <qstackedwidget.h>

// Vehicle Reminder includes
#include "mainwindow.h"
#include "vrdatabase.h"

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
};

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
    d->m_mainStackedWidget->addWidget(new QWidget(this));

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
    
    setupGUI(Default, "mainwindow.rc");
}


void MainWindow::addProfile()
{
    // TODO
}


void MainWindow::openProfile()
{
    // TODO
}


void MainWindow::removeProfile()
{
    // TODO
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
