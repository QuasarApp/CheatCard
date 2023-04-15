#
# Copyright (C) 2020-2023 QuasarApp.
# Distributed under the GPLv3 software license, see the accompanying
# Everyone is permitted to copy and distribute verbatim copies
# of this license document, but changing it is not allowed.
#

cmake_minimum_required(VERSION 3.14)

add_definitions(-DCHEATCARD_GUI_LIBRARY)
set(CURRENT_PROJECT "${PROJECT_NAME}Gui")


file(GLOB SOURCE_CPP
    "*CheatCardGui/*.cpp"
    "private/*.cpp"
    "private/platforms/*.cpp"
    "private/pills/*.cpp"
    "private/models/*.cpp"

    "*CheatCardGui/*.h"
    "private/*.h"
    "private/platforms/*.h"
    "private/pills/*.h"
    "private/models/*.h"


)

if (${QT_VERSION_MAJOR})
    file(GLOB SOURCE_QRC
        "*.qrc"
        "CheatCardGui/*.qrc"
        "private/*.qrc"
    )

endif()

set(PUBLIC_INCUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(PRIVATE_INCUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/private")


add_library(${CURRENT_PROJECT} ${SOURCE_CPP} ${SOURCE_QRC})

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Quick Svg REQUIRED)

target_link_libraries(${CURRENT_PROJECT} PUBLIC ${PROJECT_NAME}Core QmlNotyfyService SCodes Credits DoctorPillGUI Qt${QT_VERSION_MAJOR}::Svg)

target_include_directories(${CURRENT_PROJECT} PUBLIC ${PUBLIC_INCUDE_DIR})
target_include_directories(${CURRENT_PROJECT} PRIVATE ${PRIVATE_INCUDE_DIR})

set(LANGS ${CMAKE_CURRENT_SOURCE_DIR}/languages/en.ts
          ${CMAKE_CURRENT_SOURCE_DIR}/languages/ru.ts
          ${CMAKE_CURRENT_SOURCE_DIR}/languages/sr.ts
          ${CMAKE_CURRENT_SOURCE_DIR}/languages/sr_ME.ts
          ${CMAKE_CURRENT_SOURCE_DIR}/languages/uk.ts
          ${CMAKE_CURRENT_SOURCE_DIR}/languages/pl.ts
          ${CMAKE_CURRENT_SOURCE_DIR}/languages/be.ts
          ${CMAKE_CURRENT_SOURCE_DIR}/languages/ka.ts )


prepareQM(${CURRENT_PROJECT} ${CMAKE_CURRENT_SOURCE_DIR} "${LANGS}")

#set(QML_IMPORT_PATH ${QML_IMPORT_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/" CACHE STRING "" FORCE)

