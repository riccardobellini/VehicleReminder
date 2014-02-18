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

#ifndef PROFILE_H
#define PROFILE_H

// Qt includes
#include <qstring.h>
#include <QDate>
#include <qpixmap.h>


namespace structures
{
    /**
     * \struct structures::Profile
     * \brief This struct represents a profile (a user) in the application
     */
    struct Profile {
        // WARNING omit id temporarily
        QString firstName; /// first name
        QString lastName; /// last name
        QDate birthDate; /// birth date
        QString ssn; /// SSN of the profile
        QPixmap picture; /// picture of the profile
        QString licenseNumber; /// license number of the profile
        QDate issuingDate; /// issuing date of the license
        QDate licenseExpiryDate; /// next expiration date of the license
        int licenseValidityYears; /// years of validity of the license
        QString otherNotes; /// other notes related to profile
        bool notify; /// if expirations of license should be notified or not
        
        /// Overloading equality operator
        bool operator==(const Profile & other);
    };
}

#endif // PROFILE_H
