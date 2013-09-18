# Riccardo Bellini <riccardo.bellini1988@gmail.com>
# Vehicle Reminder

ADD_SUBDIRECTORY(db)
ADD_SUBDIRECTORY(structures)

INCLUDE_DIRECTORIES(db)
INCLUDE_DIRECTORIES(structures)

SET(VR_SRCS
    addprofiledialog.cpp
    mainwindow.cpp
    main.cpp
)

KDE4_ADD_UI_FILES(VR_SRCS uis/mainwidget.ui uis/addprofilewidget.ui)

KDE4_ADD_EXECUTABLE(vehiclereminder ${VR_SRCS})
TARGET_LINK_LIBRARIES(vehiclereminder ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} vrDatabase)

INSTALL(TARGETS vehiclereminder DESTINATION  ${INSTALL_TARGETS_DEFAULT_ARGS})
INSTALL(FILES mainwindow.rc DESTINATION ${DATA_INSTALL_DIR}/vehiclereminder)
