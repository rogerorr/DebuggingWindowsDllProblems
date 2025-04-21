cmake_minimum_required(VERSION 3.20)

set (CMAKE_CXX_STANDARD 23)

add_compile_options(/W4)

cmake_path(GET CMAKE_CURRENT_LIST_DIR "FILENAME" THIS_DIR)

project(MissingDll CXX)

add_library(MissingDll SHARED MissingDll.cpp)

project(${THIS_DIR} CXX)

add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cpp)
target_link_libraries(${PROJECT_NAME} MissingDll)

install(TARGETS ${PROJECT_NAME})
