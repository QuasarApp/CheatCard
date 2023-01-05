#
# Copyright (C) 2020-2023 QuasarApp.
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
if (IOS)
    option (BUILD_SHARED_LIBS "build shared libs" OFF)
else ()
    option (BUILD_SHARED_LIBS "build shared libs" ON)
endif()

if (NOT DEFINED QT_VERSION_MAJOR)
    find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Test QUIET)
endif()

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Test QUIET)

include(submodules/Heart/submodules/QuasarAppLib/CMake/QuasarApp.cmake)

initAll()
addDoc(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/doxygen.conf)


if (${QT_VERSION} VERSION_GREATER_EQUAL 6.0)
    message("This project support only qt5 or qt6 from 6.2 ")
    return()
endif()

updateGitVars()
set(CHEAT_CARD_SHORT_VERSION "1.${GIT_COMMIT_COUNT}")
set(CHEAT_CARD_VERSION "${CHEAT_CARD_SHORT_VERSION}.${GIT_COMMIT_HASH}")


if(QA_WASM32)
    return()
endif()

if (ANDROID OR IOS OR WIN32)
    option (CHEATCARD_TESTS "This option enable or disable app tests" OFF)
    option (CHEATCARD_SERVER "This option enable or disable app server" OFF)
else()
    option (CHEATCARD_TESTS "This option enable or disable app tests" ON)
    option (CHEATCARD_SERVER "This option enable or disable app server" ON)
endif()

option (CHEATCARD_APP "This option enable or disable main app" ON)
option(ALLOW_BILLING "This option force to add seller functions" ON)

add_subdirectory(submodules/Heart)

option(DOCTOR_PILL_EXAMPLE "Enable example app of this library" OFF)
option(DOCTOR_PILL_TESTS "Enable tests of this library" OFF)
add_subdirectory(submodules/DoctorPill)

add_subdirectory(src/Interfaces)
add_subdirectory(src/DB)
add_subdirectory(src/API)

add_subdirectory(src/Core)

if (CHEATCARD_SERVER)
    add_subdirectory(submodules/Patronum)
    add_subdirectory(src/Server)
else()
    message("The ${PROJECT_NAME} tests is disabled.")
endif()

if (CHEATCARD_APP)
    set(CHEATCARD_PACKAGE_ID "quasarapp.dev.cheatcard")

    add_subdirectory(submodules/SimpleQmlNotify)
    add_subdirectory(submodules/QrCodes/src)
    set(CREDITS_EXAMPLES OFF)
    add_subdirectory(submodules/Credits)
    add_subdirectory(src/GUI)
    if (ANDROID OR IOS)
        add_subdirectory(submodules/QtNativeTr)
    endif()
    add_subdirectory(src/App)
else()
    message("The ${PROJECT_NAME} app is disabled.")
endif()

if (CHEATCARD_TESTS)
    add_subdirectory(tests)
else()
    message("The ${PROJECT_NAME} tests is disabled.")
endif()
