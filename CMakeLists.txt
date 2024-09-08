cmake_minimum_required(VERSION 3.26)
project(labs)

set(CMAKE_CXX_STANDARD 23)

# Ищем библиотеку CURL
find_package(CURL REQUIRED)

# Указываем исходные файлы проекта
set(SOURCES
        main.cpp
        sqlite-amalgamation-3460100/sqlite3.c  # Убедитесь, что путь правильный
)

include_directories(${CMAKE_SOURCE_DIR}/sqlite-amalgamation-3460100)

# Добавляем исполняемый файл
add_executable(labs ${SOURCES})

# Линкуем с библиотекой CURL
target_link_libraries(labs CURL::libcurl)
