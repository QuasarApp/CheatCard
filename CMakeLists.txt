#
# Copyright (C) 2020-2021 QuasarApp.
# Distributed under the GPLv3 software license, see the accompanying
# Everyone is permitted to copy and distribute verbatim copies
# of this license document, but changing it is not allowed.
#

cmake_minimum_required(VERSION 3.14)
project(CheatCard)
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
set(BUILD_SHARED_LIBS ON)

if (NOT DEFINED QT_VERSION_MAJOR)
    find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Test QUIET)
endif()

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Test QUIET)

include(submodules/Heart/QuasarAppLib/CMake/QuasarApp.cmake)

initAll()
addDoc(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/doxygen.conf)


if (${QT_VERSION} VERSION_GREATER_EQUAL 6.0)
    message("This project support only qt5 or qt6 from 6.2 ")
    return()
endif()

updateGitVars()
set(CHEAT_CARD_VERSION "1.${GIT_COMMIT_COUNT}.${GIT_COMMIT_HASH}")

if (NOT DEFINED CHEATCARD_TESTS)
    set(CHEATCARD_TESTS ON)

    if (DEFINED TARGET_PLATFORM_TOOLCHAIN)
        if (${TARGET_PLATFORM_TOOLCHAIN} STREQUAL "wasm32")
            set(CHEATCARD_TESTS OFF)
            return()
        endif()
    endif()

    if (ANDROID OR IOS)
        set(CHEATCARD_TESTS OFF)
    endif()

    if (NOT QT_VERSION_MAJOR)
        set(CHEATCARD_TESTS OFF)
    endif()

    if(WIN32)
        set(CHEATCARD_TESTS OFF)
    endif()
endif()

if (NOT DEFINED CHEATCARD_SERVER)
    set(CHEATCARD_SERVER ON)

    if (DEFINED TARGET_PLATFORM_TOOLCHAIN)
        if (${TARGET_PLATFORM_TOOLCHAIN} STREQUAL "wasm32")
            set(CHEATCARD_SERVER OFF)
            return()
        endif()
    endif()

    if (ANDROID OR APPLE)
        set(CHEATCARD_SERVER OFF)
    endif()

    if (NOT QT_VERSION_MAJOR)
        set(CHEATCARD_SERVER OFF)
    endif()

    if(WIN32)
        set(CHEATCARD_SERVER OFF)
    endif()
endif()

if (NOT DEFINED CHEATCARD_APP)
    set(CHEATCARD_APP ON)

    if (DEFINED TARGET_PLATFORM_TOOLCHAIN)
        if (${TARGET_PLATFORM_TOOLCHAIN} STREQUAL "wasm32")
            set(CHEATCARD_APP OFF)
            return()
        endif()
    endif()

endif()

set(HEART_BUILD_LVL 1)
set(HEART_SSL ON)

add_subdirectory(submodules/Heart)
add_subdirectory(src/RC)

if (CHEATCARD_SERVER)
    add_subdirectory(submodules/Patronum)
    add_subdirectory(src/CheatCardServer)
else()
    message("The ${PROJECT_NAME} tests is disabled.")
endif()

if (CHEATCARD_TESTS)
    add_subdirectory(tests)
else()
    message("The ${PROJECT_NAME} tests is disabled.")
endif()

if (CHEATCARD_APP)
    add_subdirectory(submodules/SimpleQmlNotify)
    add_subdirectory(submodules/QrCodes/src)
    add_subdirectory(submodules/Credits)
    add_subdirectory(src/GUI)
    add_subdirectory(src/RCApp)
else()
    message("The ${PROJECT_NAME} app is disabled.")
endif()

