cmake_minimum_required(VERSION 3.20)

set (CMAKE_CXX_STANDARD 23)

add_compile_options(/W4)

cmake_path(GET CMAKE_CURRENT_LIST_DIR "FILENAME" THIS_DIR)
project(${THIS_DIR} CXX)

add_library(${THIS_DIR} SHARED ${THIS_DIR}.cpp)
set_target_properties(${THIS_DIR} PROPERTIES OUTPUT_NAME "ChangedExports")

install(TARGETS ${PROJECT_NAME})
