#
# Copyright (C) 2021-2021 QuasarApp.
# Distributed under the lgplv3 software license, see the accompanying
# Everyone is permitted to copy and distribute verbatim copies
# of this license document, but changing it is not allowed.
#

cmake_minimum_required(VERSION 3.14)

set(CURRENT_PROJECT "${PROJECT_NAME}")

file(GLOB SOURCE_CPP
    "*.cpp"
     "Private/*.cpp"
     "*.qrc"
     "Private/*.qrc"
)

set(PUBLIC_INCUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

set(PRIVATE_INCUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Private")

# Qt 6.2 hot fix
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Sql Network Concurrent QUIET)
updateGitVars()


if (${QT_VERSION_MAJOR} EQUAL 6)

    qt_add_executable(${CURRENT_PROJECT} MANUAL_FINALIZATION ${SOURCE_CPP})

else()
    if (ANDROID)
        add_library(${CURRENT_PROJECT} ${SOURCE_CPP})
    else ()
        add_executable(${CURRENT_PROJECT} ${SOURCE_CPP} )
    endif()

endif()

target_link_libraries(${CURRENT_PROJECT} PUBLIC Qt${QT_VERSION_MAJOR}::Core)

target_link_libraries(${CURRENT_PROJECT} PUBLIC "${PROJECT_NAME}Gui")

target_include_directories(${CURRENT_PROJECT} PUBLIC ${PUBLIC_INCUDE_DIR})
target_include_directories(${CURRENT_PROJECT} PRIVATE ${PRIVATE_INCUDE_DIR})


SET(TARGET_DIR "${CMAKE_SOURCE_DIR}/Distro")
file(MAKE_DIRECTORY ${TARGET_DIR})

if (ANDROID)
    set(ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android CACHE INTERNAL "")

    set(SNAKE_EXTRA_LIBS
        ${PROJECT_NAME}Core
        ${PROJECT_NAME}Gui
        QuasarApp
        Heart
        QmlNotyfyService
        SCodes
        ZXing
        )

#    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS AndroidExtras REQUIRED)
#    target_link_libraries(${CURRENT_PROJECT} PRIVATE Qt${QT_VERSION_MAJOR}::AndroidExtras)

    addDeployAPK(${CURRENT_PROJECT}
                "${CMAKE_CURRENT_LIST_DIR}/android"
                "QuasarAppProject"
                "${SIGPATH}/quasarapp.keystore"
                "${SIGPASS_QUASARAPP}"
                "${TARGET_DIR}"
                "${SNAKE_EXTRA_LIBS}")


    file(GLOB java_files
       "android/src/com/quasarapp/androidtools/*.java"
       "android/*.xml"
       "android/*.gradle"
       "android/gradlew"
       "android/gradle.*"

    )

    add_custom_target(${name}Android
        SOURCES ${java_files}
    )

else()


    message(GIT_COMMIT_COUNT = ${GIT_COMMIT_COUNT})

    set(DeployFile "${CMAKE_CURRENT_SOURCE_DIR}/../../Deploy/CheatCard.json")
    configure_file("${DeployFile}.in" ${DeployFile} @ONLY)
    addDeployFromCustomFile("CheatCard" ${DeployFile})
endif()

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(${CURRENT_PROJECT})
    qt_finalize_executable(${CURRENT_PROJECT})
endif()

