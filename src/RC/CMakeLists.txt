#
# Copyright (C) 2020-2021 QuasarApp.
# Distributed under the lgplv3 software license, see the accompanying
# Everyone is permitted to copy and distribute verbatim copies
# of this license document, but changing it is not allowed.
#

cmake_minimum_required(VERSION 3.14)

add_definitions(-DRegularCustomer_LIBRARY)


file(GLOB SOURCE_CPP
    "*RegularCustomer/*.cpp"
     "private/*.cpp"
)

if (${QT_VERSION_MAJOR})
    file(GLOB SOURCE_QRC
        "*.qrc"
        "RegularCustomer/*.qrc"
        "private/*.qrc"
    )

endif()

set(PUBLIC_INCUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(PRIVATE_INCUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/private")


add_library(${PROJECT_NAME} ${SOURCE_CPP} ${SOURCE_QRC})

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Quick REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC Heart Qt${QT_VERSION_MAJOR}::Quick)

target_include_directories(${PROJECT_NAME} PUBLIC ${PUBLIC_INCUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${PRIVATE_INCUDE_DIR})

set(LANGS ${CMAKE_CURRENT_SOURCE_DIR}/languages/en.ts)


prepareQM(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR} "${LANGS}")

set(QML_IMPORT_PATH ${QML_IMPORT_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/src" CACHE STRING "" FORCE)

