cmake_minimum_required(VERSION 3.26)
project(task_management)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
SET(CMAKE_TOOLCHAIN_FILE)
if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
endif()
find_package(tinyxml2 CONFIG REQUIRED)
find_package(CURL REQUIRED)
find_package(libpqxx CONFIG REQUIRED)
find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED
)

link_directories("C:/Users/mzitr/vcpkg/installed/x64-windows/debug/lib")

set(SOURCES

        #---------------------

        src/main.cpp
        src/sendingEmail.cpp
        src/UserManager.cpp
        src/User.cpp

        #---------------------

        header/User.h
        header/UserManager.h
        header/EmailSender.h
        header/sendingEmail.h

        #---------------------

        menu/headers/registrationwindow.h
        menu/headers/mainwindow.h
        menu/src/mainwindow.cpp
        menu/src/registrationwindow.cpp
        menu/src/registrationwindow.ui
        menu/src/companyregwindow.cpp
        menu/headers/companyregwindow.h
        menu/src/companyregwindow.ui

        #---------------------

        UsersWindows/userWindowCore/userwindow.cpp
        UsersWindows/userWindowCore/userwindow.h
        UsersWindows/userWindowCore/userwindow.ui
        UsersWindows/BasicClassCore/basicclass.cpp
        UsersWindows/BasicClassCore/basicclass.h
        UsersWindows/BasicClassCore/basicclass.ui
        UsersWindows/AdminWindowCore/adminclass.cpp
        UsersWindows/AdminWindowCore/adminclass.h
        UsersWindows/AdminWindowCore/adminclass.ui
        UsersWindows/EmployeeWindow/employeewindow.cpp

        #---------------------

        Companies/src/Company.cpp
        Companies/header/Company.h
        Companies/header/CompanyManager.h
        Companies/src/CompanyManager.cpp

        #---------------------


        Invites/src/Invite.cpp
        Invites/headers/Invite.h
        Invites/src/InviteManager.cpp
        Invites/headers/InviteManager.h

        #---------------------

        CustomException/core/LoginException.h

        #---------------------

        XmlHandler/src/XmlHandler.cpp
        XmlHandler/header/XmlHandler.h

        #---------------------

        ItemContainer/src/ItemContainer.cpp
        ItemContainer/header/ItemContainer.h

        #---------------------

        ItemsIterator/src/ItemsIterator.cpp
        ItemsIterator/header/ItemsIterator.h

        #---------------------

        UsersWindows/HandleWindow/HandleUserInfo/handleinfoclass.cpp
        UsersWindows/HandleWindow/HandleUserInfo/handleinfoclass.h
        UsersWindows/HandleWindow/HandleUserInfo/handleinfoclass.ui
        UsersWindows/HandleWindow/HandleCompanyInfoCore/handlecompanyinfo.cpp
        UsersWindows/HandleWindow/HandleCompanyInfoCore/handlecompanyinfo.h
        UsersWindows/HandleWindow/HandleCompanyInfoCore/handlecompanyinfo.ui

        #---------------------

        Task/src/Task.cpp
        Task/header/Task.h
        Task/src/TaskManager.cpp
        Task/header/TaskManager.h

        #---------------------

        Task/TaskWindow/taskwindow.cpp
        Task/TaskWindow/taskwindow.h
        Task/TaskWindow/taskwindow.ui
        UsersWindows/EmployeeWindow/ToDoListWindow/todowindow.cpp
        UsersWindows/EmployeeWindow/ToDoListWindow/todowindow.h
        UsersWindows/EmployeeWindow/ToDoListWindow/todowindow.ui
        ItemContainer/src/ItemContainer.cpp
        ItemContainer/header/ItemContainer.h

        #---------------------

        #---------------------
)

qt_add_resources(RESOURCE_FILES resources.qrc)

add_executable(task_management ${SOURCES})

target_link_libraries(task_management
        CURL::libcurl
        libpqxx::pqxx
        tinyxml2::tinyxml2
)

target_link_libraries(task_management
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

target_include_directories(task_management PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/header")
target_include_directories(task_management PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(task_management PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(task_management PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/UsersWindows")
target_include_directories(task_management PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/resources")
target_include_directories(task_management PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/menu")
target_include_directories(task_management PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Invites")

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