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
#include <qsqlquery.h>

// Vehicle Reminder includes
#include "vrdatabase.h"


const QString VRDatabase::LatestVersion = "0.1";


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


bool VRDatabase::init()
{
    // create all the tables to the latest version
    if (!m_initProfileTable()) {
        return false;
    }
    if (!m_initVehicleTypeTable()) {
        return false;
    }
    if (!m_initInsuranceInfoTable()) {
        return false;
    }
    if (!m_initVehicleTable()) {
        return false;
    }
    if (!m_initDeadlineTypeTable()) {
        return false;
    }
    if (!m_initDeadlineTable()) {
        return false;
    }
    if (!m_initRefuellingsLogTable()) {
        return false;
    }
    if (!m_initServicesLogTable()) {
        return false;
    }
    if (!m_initInsurancePaymentsLogTable()) {
        return false;
    }
    
    // if everything went correctly, return true
    return true;
}


bool VRDatabase::migrate(const QString& from, const QString& to)
{
    // TODO
    
    return true;
}


bool VRDatabase::m_initProfileTable(const QString& toVersion)
{
    // TODO
    
    return true;
}


bool VRDatabase::m_initVehicleTypeTable(const QString& toVersion)
{
    // TODO
    
    return true;
}


bool VRDatabase::m_initInsuranceInfoTable(const QString& toVersion)
{
    // TODO
    
    return true;
}


bool VRDatabase::m_initVehicleTable(const QString& toVersion)
{
    // TODO
    
    return true;
}


bool VRDatabase::m_initDeadlineTypeTable(const QString& toVersion)
{
    // TODO
    
    return true;
}


bool VRDatabase::m_initDeadlineTable(const QString& toVersion)
{
    // TODO
    
    return true;
}


bool VRDatabase::m_initRefuellingsLogTable(const QString& toVersion)
{
    // TODO
    
    return true;
}


bool VRDatabase::m_initServicesLogTable(const QString& toVersion)
{
    // TODO
    
    return true;
}


bool VRDatabase::m_initInsurancePaymentsLogTable(const QString& toVersion)
{
    // TODO
    
    return true;
}
