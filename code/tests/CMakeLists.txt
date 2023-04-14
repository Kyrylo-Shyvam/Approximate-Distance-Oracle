cmake_minimum_required(VERSION 3.1)
project(tests)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/")
set(INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../src/")

file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.cpp")

foreach(file_path ${SRC_FILES})
    string( REPLACE ".cpp" "" new_name ${file_path} )
    get_filename_component(filename ${new_name} NAME)
    add_executable( ${filename} ${file_path} )
    target_include_directories(${filename} PRIVATE "${INC_DIR}")
    target_include_directories(${filename} PRIVATE "${LIB_DIR}")
    target_link_libraries(${filename} "oracle")
    set_property(TARGET ${filename} PROPERTY CXX_STANDARD 17)
endforeach()
