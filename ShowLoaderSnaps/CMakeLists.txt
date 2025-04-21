cmake_minimum_required(VERSION 3.20)

cmake_path(GET CMAKE_CURRENT_LIST_DIR "FILENAME" THIS_DIR)
project(${THIS_DIR} CXX)

set (CMAKE_CXX_STANDARD 23)

add_compile_options(/W4)

add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cpp DebugDriver.cpp)

install(TARGETS ${PROJECT_NAME})
