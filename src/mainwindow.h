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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// KDE includes
#include <kxmlguiwindow.h>


class MainWindowPrivate;

class MainWindow : public KXmlGuiWindow
{
Q_OBJECT
public:
    MainWindow();
        
public Q_SLOTS:
    void addProfile();
    void openProfile();
    void removeProfile();
    
private:
    void m_setupActions();
    // private class for view elements
    // code inspired from Skrooge base gui
    MainWindowPrivate* const d;
};

#endif // MAINWINDOW_H
