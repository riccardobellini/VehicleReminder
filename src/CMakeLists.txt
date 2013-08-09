# Riccardo Bellini <riccardo.bellini1988@gmail.com>
# Vehicle Reminder

SET(VR_SRCS
    mainwindow.cpp
    main.cpp
)

KDE4_ADD_UI_FILES(VR_SRCS uis/mainwidget.ui)

ADD_EXECUTABLE(vehiclereminder ${VR_SRCS})
TARGET_LINK_LIBRARIES(vehiclereminder ${KDE4_KDEUI_LIBS})

INSTALL(TARGETS vehiclereminder DESTINATION  ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES mainwindow.rc DESTINATION ${DATA_INSTALL_DIR}/vehiclereminder)
