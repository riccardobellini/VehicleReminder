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


#ifndef VRDATABASE_H
#define VRDATABASE_H

#include <qobject.h>
#include <qsqldatabase.h>


class QSqlDatabase;

class VRDatabase : public QObject
{
public:
    explicit VRDatabase(QObject *parent = 0);
    
    bool open(const QString &dbPath = QString());
    
private:
    QSqlDatabase m_database;
    QString m_dbPath;
};

#endif // VRDATABASE_H
