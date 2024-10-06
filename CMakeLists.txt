cmake_minimum_required(VERSION 3.26)
project(task_management)

set(CMAKE_CXX_STANDARD 23)
find_package(CURL REQUIRED)

# Явно указываем исходные файлы
set(SOURCES
        src/main.cpp
        src/sendingEmail.cpp

        #-------------------

        header/User.h
        header/UserManager.h
        header/EmailSender.h
        header/sendingEmail.h

        #--------------------

        include/sqlite/sqlite3.c
        include/sqlite/sqlite3.h

)

# Создаем исполняемый файл
add_executable(task_management ${SOURCES})

# Подключаем библиотеку CURL
target_link_libraries(task_management CURL::libcurl)

# Указываем директории
target_include_directories(task_management PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/header")
target_include_directories(task_management PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(task_management PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")



