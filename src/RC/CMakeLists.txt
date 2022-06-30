#
# Copyright (C) 2020-2021 QuasarApp.
# Distributed under the GPLv3 software license, see the accompanying
# Everyone is permitted to copy and distribute verbatim copies
# of this license document, but changing it is not allowed.
#

cmake_minimum_required(VERSION 3.14)

add_definitions(-DCHEATCARD_CORE_LIBRARY)
set(CURRENT_PROJECT "${PROJECT_NAME}Core")

updateGitVars()

file(GLOB SOURCE_CPP
    "*CheatCard/*.cpp"
     "private/*.cpp"
     "CheatCard/pills/*.cpp"
     "CheatCard/api/*.cpp"
     "CheatCard/api/api0/*.cpp"
     "CheatCard/api/api1/*.cpp"
     "CheatCard/api/api1-5/*.cpp"

     "*CheatCard/*.h"
      "private/*.h"
      "CheatCard/pills/*.h"
      "CheatCard/api/*.h"
      "CheatCard/api/api0/*.h"
      "CheatCard/api/api1/*.h"
      "CheatCard/api/api1-5/*.h"

)

if (${QT_VERSION_MAJOR})
    file(GLOB SOURCE_QRC
        "*.qrc"
        "CheatCard/*.qrc"
        "private/*.qrc"
        "CheatCard/api/*.qrc"
    )

endif()

set(PUBLIC_INCUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(PRIVATE_INCUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/private")


add_library(${CURRENT_PROJECT} ${SOURCE_CPP} ${SOURCE_QRC})

target_link_libraries(${CURRENT_PROJECT} PUBLIC Heart DoctorPill)

target_include_directories(${CURRENT_PROJECT} PUBLIC ${PUBLIC_INCUDE_DIR})
target_include_directories(${CURRENT_PROJECT} PRIVATE ${PRIVATE_INCUDE_DIR})


#set(QML_IMPORT_PATH ${QML_IMPORT_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/" CACHE STRING "" FORCE)

set(global_file "${CMAKE_CURRENT_SOURCE_DIR}/CheatCard/core_global.h")
configure_file("${global_file}.in" ${global_file} @ONLY)
