# Riccardo Bellini <riccardo.bellini1988@gmail.com>
# Vehicle Reminder

FIND_PACKAGE(Qt4 COMPONENTS QtSql REQUIRED)

INCLUDE_DIRECTORIES(../structures)

SET(VRDB_SRCS
    vrdatabase.cpp
)

KDE4_ADD_LIBRARY(vrDatabase ${VRDB_SRCS})

TARGET_LINK_LIBRARIES(vrDatabase ${QT_QTSQL_LIBRARY} ${QT_QTCORE_LIBRARY})
