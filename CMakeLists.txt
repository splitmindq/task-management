cmake_minimum_required(VERSION 3.26)
project(task_management)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_TOOLCHAIN_FILE "C:/Users/mzitr/vcpkg/scripts/buildsystems/vcpkg.cmake")

find_package(CURL REQUIRED)
find_package(libpqxx CONFIG REQUIRED)


link_directories("C:/Users/mzitr/vcpkg/installed/x64-windows/debug/lib")

# Явно указываем исходные файлы
set(SOURCES

        src/main.cpp
        src/sendingEmail.cpp
        src/Session.cpp
        src/UserManager.cpp
        src/User.cpp

        #-------------------

        header/User.h
        header/UserManager.h
        header/EmailSender.h
        header/sendingEmail.h
        header/Session.h

        #--------------------


)

# Создаем исполняемый файл
add_executable(task_management ${SOURCES})

# Подключаем библиотеку CURL
target_link_libraries(task_management CURL::libcurl pqxx)
#target_link_libraries(task_management PRIVATE curlcpp::curlcpp)


# Указываем директории
target_include_directories(task_management PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/header")
target_include_directories(task_management PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(task_management PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")



