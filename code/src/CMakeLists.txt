cmake_minimum_required(VERSION 3.1)
project(oracle)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/")
set(INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/")
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")

add_library(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE "${INC_DIR}")
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
