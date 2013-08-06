# Riccardo Bellini <riccardo.bellini1988@gmail.com>
# Vehicle Reminder

SET(VR_SRCS
    main.cpp
)

ADD_EXECUTABLE(vehiclereminder ${VR_SRCS})
TARGET_LINK_LIBRARIES(vehiclereminder ${KDE4_KDEUI_LIBS})

INSTALL(TARGETS vehiclereminder DESTINATION  ${INSTALL_TARGETS_DEFAULT_ARGS})
