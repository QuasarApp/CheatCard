#
# Copyright (C) 2020-2023 QuasarApp.
# Distributed under the GPLv3 software license, see the accompanying
# Everyone is permitted to copy and distribute verbatim copies
# of this license document, but changing it is not allowed.
#

cmake_minimum_required(VERSION 3.18)

get_filename_component(CURRENT_PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR} NAME)

add_definitions(-DCHEATCARD_INTERFACES_LIBRARY)
set(CURRENT_PROJECT "${PROJECT_NAME}${CURRENT_PROJECT_DIR}")

updateGitVars()

file(GLOB SOURCE_CPP
    "public/rci/*.h"
    "public/rci/core/*.h"
    "public/rci/objects/*.h"
    "public/*.h"

    "public/rci/*.cpp"
    "public/rci/core/*.cpp"
    "public/rci/objects/*.cpp"
    "public/*.cpp"
)

set(PUBLIC_INCUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/public")
set(PRIVATE_INCUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/private")


add_library(${CURRENT_PROJECT} ${SOURCE_CPP} )

target_link_libraries(${CURRENT_PROJECT} PUBLIC QuasarApp crc)

target_include_directories(${CURRENT_PROJECT} PUBLIC ${PUBLIC_INCUDE_DIR})
target_include_directories(${CURRENT_PROJECT} PRIVATE ${PRIVATE_INCUDE_DIR})


set(global_core_file "${CMAKE_CURRENT_SOURCE_DIR}/public/rci/global.h")
configure_file_in(${CURRENT_PROJECT} ${global_core_file})
