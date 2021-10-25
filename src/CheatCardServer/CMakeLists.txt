#
# Copyright (C) 2021-2021 QuasarApp.
# Distributed under the lgplv3 software license, see the accompanying
# Everyone is permitted to copy and distribute verbatim copies
# of this license document, but changing it is not allowed.
#

cmake_minimum_required(VERSION 3.14)

set(CURRENT_PROJECT "${PROJECT_NAME}Server")

file(GLOB SOURCE_CPP
    "*.cpp"
     "Private/*.cpp"
     "*.qrc"
     "Private/*.qrc"
)

updateGitVars()

set(PUBLIC_INCUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

set(PRIVATE_INCUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Private")

# Qt 6.2 hot fix
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Sql Network Concurrent QUIET)

add_executable(${CURRENT_PROJECT} ${SOURCE_CPP} )

target_link_libraries(${CURRENT_PROJECT} PUBLIC Qt${QT_VERSION_MAJOR}::Core)

target_link_libraries(${CURRENT_PROJECT} PUBLIC "${PROJECT_NAME}Core" Patronum)

target_include_directories(${CURRENT_PROJECT} PUBLIC ${PUBLIC_INCUDE_DIR})
target_include_directories(${CURRENT_PROJECT} PRIVATE ${PRIVATE_INCUDE_DIR})


SET(TARGET_DIR "${CMAKE_SOURCE_DIR}/Distro")
file(MAKE_DIRECTORY ${TARGET_DIR})

set(DeployFile "${CMAKE_CURRENT_SOURCE_DIR}/Deploy/CheatCardServer.json")
configure_file("${DeployFile}.in" ${DeployFile} @ONLY)
addDeployFromCustomFile("CheatCardServer" ${DeployFile})

