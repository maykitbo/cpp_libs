cmake_minimum_required(VERSION 3.22)

project(thread_pool.a VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} STATIC
        thread_pool.h
        dummy.cc
        )

set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME thread_pool
        PREFIX ""
        )

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})