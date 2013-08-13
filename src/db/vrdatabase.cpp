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
#include <kstandarddirs.h>

// Qt includes


// Vehicle Reminder includes
#include "vrdatabase.h"


VRDatabase::VRDatabase(QObject *parent) : QObject(parent)
{
    QString type = "QSQLITE";
    m_database = QSqlDatabase::addDatabase(type);
}


bool VRDatabase::open(const QString &dbPath)
{
    if (dbPath.isEmpty()) {
        m_dbPath = KStandardDirs::locateLocal("appdata", "vr.db");
    } else {
        m_dbPath = dbPath;
    }
    m_database.setDatabaseName(m_dbPath);
    return m_database.open();
}
