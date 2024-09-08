cmake_minimum_required(VERSION 3.26)
project(labs)

set(CMAKE_CXX_STANDARD 23)

find_package(CURL REQUIRED)

set(SOURCES
        main.cpp
        sqlite-amalgamation-3460100/sqlite3.c
        sendingEmail.cpp
        sendingEmail.h
)

include_directories(${CMAKE_SOURCE_DIR}/sqlite-amalgamation-3460100)

add_executable(labs ${SOURCES}
        sendingEmail.cpp
        sendingEmail.h)

target_link_libraries(labs CURL::libcurl)
