cmake_minimum_required(VERSION 3.0)

project(utitity VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME} STATIC
        sstr.h
        time.h
        dummy.cc
        random.h)