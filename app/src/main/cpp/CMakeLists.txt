cmake_minimum_required(VERSION 3.22.1)

project("DoodleJumpCpp")

add_subdirectory(src/engine)

add_library(${CMAKE_PROJECT_NAME} SHARED
        src/native-lib.cpp)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE include)

target_link_libraries(${CMAKE_PROJECT_NAME}
        Engine
        android
        log)