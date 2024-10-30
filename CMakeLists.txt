cmake_minimum_required(VERSION 3.26)
project(task_management)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_TOOLCHAIN_FILE "C:/Users/mzitr/vcpkg/scripts/buildsystems/vcpkg.cmake")

find_package(CURL REQUIRED)
find_package(libpqxx CONFIG REQUIRED)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

link_directories("C:/Users/mzitr/vcpkg/installed/x64-windows/debug/lib")

set(SOURCES
        src/main.cpp
        src/sendingEmail.cpp
        src/Session.cpp
        src/UserManager.cpp
        src/User.cpp
        menu/src/mainwindow.cpp
        UsersWindows/src/userwindow.cpp
        #---------------------

        header/User.h
        header/UserManager.h
        header/EmailSender.h
        header/sendingEmail.h
        header/Session.h
        UsersWindows/headers/userwindow.h

        #---------------------

        UsersWindows/ui/userwindow.ui
        menu/ui/mainwindow.ui

)
qt6_add_resources(RESOURCE_TARGET "${CMAKE_CURRENT_SOURCE_DIR}/resources.qrc")

add_executable(task_management ${SOURCES})

target_link_libraries(task_management CURL::libcurl libpqxx::pqxx)

target_link_libraries(task_management
        Qt::Core
        Qt::Gui
        Qt::Widgets
)
target_include_directories(task_management PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/header")
target_include_directories(task_management PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(task_management PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(task_management PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/UsersWindows")
target_include_directories(task_management PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/recources")
target_include_directories(task_management PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/menu")

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
