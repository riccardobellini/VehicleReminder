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
#include <kdebug.h>

// Qt includes
#include <qsqlquery.h>
#include <qsqlerror.h>

// Vehicle Reminder includes
#include "vrdatabase.h"


using structures::Profile;


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


bool VRDatabase::insertNewProfile(const Profile& profile)
{
    QSqlQuery insertQuery(m_database);
    
    insertQuery.prepare("INSERT INTO profile (first_name, last_name, birthdate, ssn, picture, "
        "license_number, issuing_date, license_expiry, license_validity_years, other_notes, notify) "
        "VALUES (:first_name, :last_name, :birthdate, :ssn, :picture, :license_number, :issuing_date, "
        ":license_expiry, :license_validity_years, :other_notes, :notify)");
    insertQuery.bindValue(0, profile.firstName);
    insertQuery.bindValue(1, profile.lastName);
    insertQuery.bindValue(2, profile.birthDate);
    insertQuery.bindValue(3, profile.ssn);
    insertQuery.bindValue(4, profile.picture);
    insertQuery.bindValue(5, profile.licenseNumber);
    insertQuery.bindValue(6, profile.issuingDate);
    insertQuery.bindValue(7, profile.licenseExpiryDate);
    insertQuery.bindValue(8, profile.licenseValidityYears);
    insertQuery.bindValue(9, profile.otherNotes);
    insertQuery.bindValue(10, profile.notify);
    
    // FIXME check for empty values?
    
    insertQuery.exec();
    if (!insertQuery.isActive()) {
        kError() << insertQuery.lastError().text();
        return false;
    }
    
    return true;
}


bool VRDatabase::m_initProfileTable(const QString& toVersion)
{
    QSqlQuery query(m_database);
    
    if (toVersion == LatestVersion) {
        // need to create the latest version of the table
        QString command = QString(
            "CREATE TABLE profile ("
            "id INTEGER PRIMARY KEY, "
            "first_name VARCHAR(50) NOT NULL, "
            "last_name VARCHAR(50) NOT NULL, "
            "birthdate DATE NOT NULL, "
            "ssn VARCHAR(40), "
            "picture BLOB, "
            "license_number VARCHAR(30) NOT NULL, "
            "issuing_date DATE, "
            "license_expiry DATE, "
            "license_validity_years INTEGER, "
            "other_notes TEXT, "
            "notify BOOLEAN DEFAULT 1)");
        
        query.exec(command);
        if (!query.isActive()) {
            kError() << query.lastError().text();
            return false;
        }
    }
    
    return true;
}


bool VRDatabase::m_initVehicleTypeTable(const QString& toVersion)
{
    QSqlQuery query(m_database);
    
    if (toVersion == LatestVersion) {
        // need to create the latest version of the table
        QString command = QString(
            "CREATE TABLE vehicle_info ("
            "id INTEGER PRIMARY KEY, "
            "make VARCHAR(30) NOT NULL, "
            "model VARCHAR(30) NOT NULL, "
            "trim VARCHAR(50), "
            "year INTEGER DEFAULT NULL, "
            "body VARCHAR(30) DEFAULT NULL, "
            "engine_position VARCHAR(20) DEFAULT NULL, "
            "engine_cc INTEGER DEFAULT NULL, "
            "engine_cyl INTEGER DEFAULT NULL, "
            "engine_type VARCHAR(30) DEFAULT NULL, "
            "engine_valves_per_cyl INTEGER DEFAULT NULL, "
            "engine_power_ps INTEGER DEFAULT NULL, "
            "engine_power_rpm INTEGER DEFAULT NULL, "
            "engine_torque_nm INTEGER DEFAULT NULL, "
            "engine_torque_rpm INTEGER DEFAULT NULL, "
            "engine_bore_mm FLOAT DEFAULT NULL, "
            "engine_stroke_mm FLOAT DEFAULT NULL, "
            "engine_compression VARCHAR(10) DEFAULT NULL, "
            "engine_fuel VARCHAR(30) DEFAULT NULL, "
            "drive VARCHAR(30) DEFAULT NULL, "
            "transmission_type VARCHAR(30) DEFAULT NULL, "
            "seats INTEGER DEFAULT NULL, "
            "doors INTEGER DEFAULT NULL, "
            "weight_kg INTEGER DEFAULT NULL, "
            "length_mm INTEGER DEFAULT NULL, "
            "width_mm INTEGER DEFAULT NULL, "
            "height_mm INTEGER DEFAULT NULL, "
            "wheelbase_mm INTEGER DEFAULT NULL, "
            "lkm_hwy FLOAT DEFAULT NULL, "
            "lkm_mixed FLOAT DEFAULT NULL, "
            "lkm_city FLOAT DEFAULT NULL, "
            "fuel_cap_l INTEGER DEFAULT NULL, "
            "country VARCHAR(50) DEFAULT NULL, "
            "engine_l FLOAT DEFAULT NULL, "
            "engine_ci INTEGER DEFAULT NULL, "
            "engine_bore_in FLAOT DEFAULT NULL, "
            "engine_stroke_in FLOAT DEFAULT NULL, "
            "engine_valves INTEGER DEFAULT NULL, "
            "engine_power_hp INTEGER DEFAULT NULL, "
            "engine_power_kw INTEGER DEFAULT NULL, "
            "engine_torque_lbft INTEGER DEFAULT NULL, "
            "engine_torque_kgm FLOAT DEFAULT NULL, "
            "weight_lbs INTEGER DEFAULT NULL, "
            "length_in FLOAT DEFAULT NULL, "
            "width_in FLOAT DEFAULT NULL, "
            "height_in FLOAT DEFAULT NULL, "
            "wheelbase_in FLOAT DEFAULT NULL, "
            "mpg_hwy INTEGER DEFAULT NULL, "
            "mpg_city INTEGER DEFAULT NULL, "
            "mpg_mixed INTEGER DEFAULT NULL, "
            "fuel_cap_gal FLOAT DEFAULT NULL)");
        
        query.exec(command);
        if (!query.isActive()) {
            kError() << query.lastError().text();
            return false;
        }
    }
    
    return true;
}


bool VRDatabase::m_initInsuranceInfoTable(const QString& toVersion)
{
    QSqlQuery query(m_database);
    
    if (toVersion == LatestVersion) {
        // need to create the latest version of the table
        QString command = QString(
            "CREATE TABLE insurance_info ("
            "id INTEGER PRIMARY KEY, "
            "company_name VARCHAR(50), "
            "account_no VARCHAR(30), "
            "premium FLOAT DEFAULT 0.0, "
            "next_due DATE, "
            "period_unit INTEGER DEFAULT 1, "
            "period_measure VARCHAR(10) DEFAULT 'Years', "
            "notify BOOLEAN DEFAULT 1)");
        
        query.exec(command);
        if (!query.isActive()) {
            kError() << query.lastError().text();
            return false;
        }
    }
    
    return true;
}


bool VRDatabase::m_initVehicleTable(const QString& toVersion)
{
    QSqlQuery query(m_database);
    
    if (toVersion == LatestVersion) {
        // need to create the latest version of the table
        QString command = QString(
            "CREATE TABLE vehicle ("
            "id INTEGER PRIMARY KEY, "
            "vehicle_info INTEGER, "
            "owner INTEGER, "
            "registration_plate VARCHAR(30) NOT NULL, "
            "purchase_date DATE, "
            "purchase_price FLOAT, "
            "annotations TEXT, "
            "odometer_km FLOAT DEFAULT 0.0, "
            "odometer_mi FLOAT DEFAULT 0.0, "
            "insurance_info INTEGER, "
            "picture BLOB, "
            "notify BOOLEAN NOT NULL, "
            "FOREIGN KEY(owner) REFERENCES profile(id) ON DELETE CASCADE ON UPDATE CASCADE, "
            "FOREIGN KEY(vehicle_info) REFERENCES vehicle_info(id), "
            "FOREIGN KEY(insurance_info) REFERENCES insurance_info(id))");
        
        query.exec(command);
        if (!query.isActive()) {
            kError() << query.lastError().text();
            return false;
        }
    }
    
    return true;
}


bool VRDatabase::m_initDeadlineTypeTable(const QString& toVersion)
{
    QSqlQuery query(m_database);
    
    if (toVersion == LatestVersion) {
        // need to create the latest version of the table
        QString command = QString(
            "CREATE TABLE deadline_info ("
            "id INTEGER PRIMARY KEY,"
            "name VARCHAR(20) NOT NULL,"
            "description VARCHAR(100) DEFAULT NULL,"
            "type VARCHAR(10) DEFAULT 'Periodic')");
        
        query.exec(command);
        if (!query.isActive()) {
            kError() << query.lastError().text();
            return false;
        }
    }
    
    return true;
}


bool VRDatabase::m_initDeadlineTable(const QString& toVersion)
{
    QSqlQuery query(m_database);
    
    if (toVersion == LatestVersion) {
        // need to create the latest version of the table
        QString command = QString(
            "CREATE TABLE deadline ("
            "deadline_type INTEGER,"
            "vehicle INTEGER,"
            "period_unit INTEGER DEFAULT 1,"
            "period_measure VARCHAR(10) DEFAULT 'Years',"
            "every_km INTEGER,"
            "every_mi INTEGER,"
            "due_date DATE,"
            "due_km INTEGER,"
            "due_mi INTEGER,"
            "notify BOOLEAN DEFAULT 1,"
            "PRIMARY KEY(deadline_type, vehicle),"
            "FOREIGN KEY(deadline_type) REFERENCES deadline_info(id) ON DELETE CASCADE ON UPDATE CASCADE,"
            "FOREIGN KEY(vehicle) REFERENCES vehicle(id) ON DELETE CASCADE ON UPDATE CASCADE)");
        
        query.exec(command);
        if (!query.isActive()) {
            kError() << query.lastError().text();
            return false;
        }
    }
    
    return true;
}


bool VRDatabase::m_initRefuellingsLogTable(const QString& toVersion)
{
    QSqlQuery query(m_database);
    
    if (toVersion == LatestVersion) {
        // need to create the latest version of the table
        QString command = QString(
            "CREATE TABLE refuelling_log ("
            "id INTEGER PRIMARY KEY,"
            "vehicle INTEGER,"
            "refuel_l FLOAT,"
            "refuel_gal FLOAT,"
            "refuel_kg FLOAT,"
            "refuel_kwh FLOAT,"
            "cost_per_unit FLOAT,"
            "total_cost FLOAT,"
            "odometer_km FLOAT,"
            "odometer_mi FLOAT,"
            "refuel_date DATE,"
            "FOREIGN KEY(vehicle) REFERENCES vehicle(id))");
        
        query.exec(command);
        if (!query.isActive()) {
            kError() << query.lastError().text();
            return false;
        }
    }
    
    return true;
}


bool VRDatabase::m_initServicesLogTable(const QString& toVersion)
{
    QSqlQuery query(m_database);
    
    if (toVersion == LatestVersion) {
        // need to create the latest version of the table
        QString command = QString(
            "CREATE TABLE service_log ("
            "id INTEGER PRIMARY KEY,"
            "vehicle INTEGER,"
            "deadline INTEGER,"
            "date DATE,"
            "cost FLOAT,"
            "odometer_km FLOAT,"
            "odometer_mi FLOAT,"
            "FOREIGN KEY(vehicle) REFERENCES deadline(vehicle) ON DELETE CASCADE ON UPDATE CASCADE,"
            "FOREIGN KEY(deadline) REFERENCES deadline(deadline_type) ON DELETE CASCADE ON UPDATE CASCADE)");
        
        query.exec(command);
        if (!query.isActive()){
            kError() << query.lastError().text();
            return false;
        }
    }
    
    return true;
}


bool VRDatabase::m_initInsurancePaymentsLogTable(const QString& toVersion)
{
    QSqlQuery query(m_database);
    
    if (toVersion == LatestVersion) {
        // need to create the latest version of the table
        QString command = QString(
            "CREATE TABLE insurance_payments_log ("
            "id INTEGER PRIMARY KEY,"
            "vehicle INTEGER,"
            "insurance_info INTEGER,"
            "date DATE,"
            "cost FLOAT,"
            "FOREIGN KEY(vehicle) REFERENCES vehicle(id) ON DELETE CASCADE ON UPDATE CASCADE,"
            "FOREIGN KEY(insurance_info) REFERENCES insurance_info(id) ON DELETE CASCADE ON UPDATE CASCADE)");
        
        query.exec(command);
        if (!query.isActive()) {
            kError() << query.lastError().text();
            return false;
        }
    }
    
    return true;
}
