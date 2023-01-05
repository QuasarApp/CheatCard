#
# Copyright (C) 2020-2023 QuasarApp.
# Distributed under the GPLv3 software license, see the accompanying
# Everyone is permitted to copy and distribute verbatim copies
# of this license document, but changing it is not allowed.
#

cmake_minimum_required(VERSION 3.18)

get_filename_component(CURRENT_PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR} NAME)

add_definitions(-DCHEATCARD_DB_LIBRARY)
set(CURRENT_PROJECT "${PROJECT_NAME}${CURRENT_PROJECT_DIR}")

updateGitVars()

file(GLOB SOURCE_CPP
    "public/rcdb/*.h"
    "public/*.h"

    "private/pills/*.h"
    "private/*.h"
    "private/objects/*.h"

    "public/rcdb/*.cpp"
    "public/*.cpp"

    "private/*.cpp"
    "private/pills/*.cpp"
    "private/objects/*.cpp"

    "*.qrc"
)

set(PUBLIC_INCUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/public")
set(PRIVATE_INCUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/private")


add_library(${CURRENT_PROJECT} ${SOURCE_CPP} )

target_link_libraries(${CURRENT_PROJECT} PUBLIC Heart DoctorPill CheatCardInterfaces)

target_include_directories(${CURRENT_PROJECT} PUBLIC ${PUBLIC_INCUDE_DIR})
target_include_directories(${CURRENT_PROJECT} PRIVATE ${PRIVATE_INCUDE_DIR})


set(global_core_file "${CMAKE_CURRENT_SOURCE_DIR}/public/rcdb/global.h")
configure_file_in(${CURRENT_PROJECT} ${global_core_file})
