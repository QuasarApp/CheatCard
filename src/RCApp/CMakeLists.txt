#
# Copyright (C) 2021-2021 QuasarApp.
# Distributed under the GPLv3 software license, see the accompanying
# Everyone is permitted to copy and distribute verbatim copies
# of this license document, but changing it is not allowed.
#

cmake_minimum_required(VERSION 3.14)

set(CURRENT_PROJECT "${PROJECT_NAME}")
option(SIGN_APP "This option enable od disabled sign apk and aab files" ON)

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
    set(ENV{ANDROID_API_VERSION} 31)
    set(ANDROID_API_VERSION $ENV{ANDROID_API_VERSION})
    set(OPENSSL_ROOT_PATH "$ENV{OPENSSL_ROOT_DIR}")

    set(SNAKE_EXTRA_LIBS
        ${PROJECT_NAME}Core
        ${PROJECT_NAME}Gui
        QuasarApp
        Credits
        Heart
        QmlNotyfyService
        SCodes
        ZXing
        ${OPENSSL_ROOT_PATH}/lib/libcrypto_1_1.so
        ${OPENSSL_ROOT_PATH}/lib/libssl_1_1.so
        )


    if (SIGN_APP)
        message("SIGN_APP")
        addDeploySignedAPK(${CURRENT_PROJECT}
            "${CMAKE_CURRENT_LIST_DIR}/android"
            "QuasarAppProject"
            "${SIGPATH}/quasarapp.keystore"
            "${SIGPASS_QUASARAPP}"
            "${TARGET_DIR}"
            "${SNAKE_EXTRA_LIBS}")

        add_definitions(-DUSE_SIGN_APP)

    else()
        message("NO_SIGN_APP")

        addDeployAPK(${CURRENT_PROJECT}
            "${CMAKE_CURRENT_LIST_DIR}/android"
            "${TARGET_DIR}"
            "${SNAKE_EXTRA_LIBS}")

    endif()

    set(manifest_file "${CMAKE_CURRENT_SOURCE_DIR}/android/AndroidManifest.xml")
    configure_file("${manifest_file}.in" ${manifest_file} @ONLY)

    set(gradle_file "${CMAKE_CURRENT_SOURCE_DIR}/android/build.gradle")
    configure_file("${gradle_file}.in" ${gradle_file} @ONLY)

    file(GLOB java_files
        "android/src/com/quasarapp/androidtools/*.java"
        "android/*.xml"
        "android/*.in"
        "android/*.gradle"
        "android/gradlew"
        "android/gradle.*"

        )

    add_custom_target(${name}Android
        SOURCES ${java_files}
        )

elseif(IOS)
#    set_xcode_property(${CURRENT_PROJECT} PRODUCT_BUNDLE_IDENTIFIER ${CHEATCARD_PACKAGE_ID} All)

    set(QT_IOS_EXPORT_ARCHIVE_XCODEBUILD_FLAGS "-allowProvisioningUpdates")
    addDeployIPA(${CURRENT_PROJECT} "${TARGET_DIR}" "${CHEAT_CARD_SHORT_VERSION}" "${CMAKE_CURRENT_SOURCE_DIR}/apple")

#    set_target_properties(${CURRENT_PROJECT} PROPERTIES
#        MACOSX_BUNDLE TRUE
#        MACOSX_BUNDLE_INFO_PLIST ${aple_file}
#        MACOSX_BUNDLE_GUI_IDENTIFIER ${CHEATCARD_PACKAGE_ID}
#        MACOSX_BUNDLE_BUNDLE_VERSION ${CHEAT_CARD_SHORT_VERSION}
#        MACOSX_BUNDLE_SHORT_VERSION_STRING ${CHEAT_CARD_SHORT_VERSION}
#        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "Apple Development"
#        XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Automatic"
#    )
else()


    message(GIT_COMMIT_COUNT = ${GIT_COMMIT_COUNT})

    set(DeployFile "${CMAKE_CURRENT_SOURCE_DIR}/Deploy/CheatCard.json")
    configure_file("${DeployFile}.in" ${DeployFile} @ONLY)
    addDeployFromCustomFile("CheatCard" ${DeployFile})
endif()

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(${CURRENT_PROJECT})
    qt_finalize_executable(${CURRENT_PROJECT})
endif()

