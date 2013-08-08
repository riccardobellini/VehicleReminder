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

#include <kaboutdata.h>
#include <klocalizedstring.h>
#include <kcmdlineargs.h>
#include <kuniqueapplication.h>
#include <kapplication.h>
#include <kdebug.h>

#include "mainwindow.h"


static const char *version = "0.1";

int main(int argc, char **argv)
{
    KAboutData about("vehiclereminder", "", ki18n("Vehicle Reminder"), version,
                     ki18n("KDE software to manage your vehicles and keep "
                     "track of deadlines and expenses"), KAboutData::License_GPL_V2);
    about.setCopyrightStatement(ki18n("Copyright (C) 2013 Riccardo Bellini"));
    about.setHomepage("https://github.com/riccardobellini/VehicleReminder");
    about.setBugAddress("https://github.com/riccardobellini/VehicleReminder/issues");
    about.addAuthor(ki18n("Riccardo Bellini"), ki18n("Creator & Developer"),
                    "riccardo.bellini1988@gmail.com");
    
    KCmdLineArgs::init(argc, argv, &about);
    
    if (!KUniqueApplication::start()) {
        kError() << "Vehicle Reminder is already running!";
        return 0;
    }
    
    KUniqueApplication app;
    
    MainWindow *w = new MainWindow;
    w->show();
    
    return app.exec();
}
