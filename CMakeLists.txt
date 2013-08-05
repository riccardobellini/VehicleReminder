# Riccardo Bellini <riccardo.bellini1988@gmail.com>
# Vehicle Reminder

PROJECT("Vehicle Reminder")


FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui QtSql REQUIRED)
FIND_PACKAGE(KDE4 REQUIRED)


INCLUDE(KDE4Defaults)


ADD_SUBDIRECTORY(src)

