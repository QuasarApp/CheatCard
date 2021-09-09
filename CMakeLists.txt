#
# Copyright (C) 2020-2021 QuasarApp.
# Distributed under the lgplv3 software license, see the accompanying
# Everyone is permitted to copy and distribute verbatim copies
# of this license document, but changing it is not allowed.
#

cmake_minimum_required(VERSION 3.14)
project(RegularCustomer)
if(TARGET ${PROJECT_NAME})
  message("The ${PROJECT_NAME} arledy included in main Project")
  return()
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT DEFINED QT_VERSION_MAJOR)
    find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Test QUIET)
endif()

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Test QUIET)

include(submodules/Heart/QuasarAppLib/CMake/QuasarApp.cmake)

if (QT_VERSION_MAJOR LESS_EQUAL 5)
    message("The ${PROJECT_NAME} requires qt6, project build is skipped " )
    initAll()
    return()
endif()

if (NOT DEFINED RegularCustomer_TESTS)
    set(RegularCustomer_TESTS ON)

    if (DEFINED TARGET_PLATFORM_TOOLCHAIN)
        if (${TARGET_PLATFORM_TOOLCHAIN} STREQUAL "wasm32")
            set(RegularCustomer_TESTS OFF)
            initAll()
            return()
        endif()
    endif()

    if (ANDROID)
        set(RegularCustomer_TESTS OFF)
    endif()

    if (NOT QT_VERSION_MAJOR)
        set(RegularCustomer_TESTS OFF)
    endif()
endif()

set(HEART_BUILD_LVL 1)
set(HEART_SSL OFF)
add_subdirectory(submodules/Heart)

add_subdirectory(src/RC)
add_subdirectory(src/RCApp)


if (RegularCustomer_TESTS)
    add_subdirectory(tests)
else()
    message("The ${PROJECT_NAME} tests is disabled.")
endif()

initAll()

addDoc(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/doxygen.conf)
addDeployFromCustomFile("RegularCustomer" "${CMAKE_CURRENT_SOURCE_DIR}/Deploy/RegularCustomer.json")
