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

// Qt includes
#include <qscrollarea.h>
#include <QHBoxLayout>
#include <qsplitter.h>

// Vehicle Reminder includes
#include "mainwindow.h"

// Uis includes
#include "ui_mainwidget.h"

class MainWindowPrivate
{
public:
    Ui::mainWidget ui;
    QHBoxLayout *m_mainLayout;
    QWidget *m_mainWidget;
    QSplitter *m_splitter;
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
    d->m_mainWidget = new QWidget(area);

    // FIXME size of widgets
    d->m_splitter = new QSplitter(Qt::Horizontal, area);
    d->m_splitter->addWidget(d->ui.dockWidget);
    d->m_splitter->setStretchFactor(0, 1);
    d->m_splitter->addWidget(d->m_mainWidget);
    d->m_splitter->setStretchFactor(1, 2);
    
    d->m_mainLayout->addWidget(d->m_splitter);
    
    QPalette palette = QApplication::palette();
    palette.setColor(QPalette::Base, Qt::transparent);
    d->ui.klistwidget->setPalette(palette);
    d->ui.klistwidget->setFrameShape(QFrame::NoFrame);
    
    setCentralWidget(area);
    
    setupGUI(Default, "mainwindow.rc");
}

