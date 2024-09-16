cmake_minimum_required(VERSION 3.26)
project(labs)

set(CMAKE_CXX_STANDARD 23)

find_package(CURL REQUIRED)

set(SOURCES
        src/main.cpp
        src/sendingEmail.cpp
        src/sendingEmail.h
        src/user.h
        src/user_manager.h
        src/email_sender.h
)

include_directories(include)

add_executable(labs ${SOURCES})

target_link_libraries(labs CURL::libcurl)
target_include_directories(labs PRIVATE include)