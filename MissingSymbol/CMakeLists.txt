cmake_minimum_required(VERSION 3.20)

add_subdirectory(BadDll)

set (CMAKE_CXX_STANDARD 23)

add_compile_options(/W4)

cmake_path(GET CMAKE_CURRENT_LIST_DIR "FILENAME" THIS_DIR)

project(GoodDll CXX)

add_library(GoodDll SHARED GoodDll.cpp)
set_target_properties(GoodDll PROPERTIES OUTPUT_NAME "ChangedExports")

project(${THIS_DIR} CXX)

add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cpp)
target_link_libraries(${PROJECT_NAME} GoodDll)

install(TARGETS ${PROJECT_NAME})
