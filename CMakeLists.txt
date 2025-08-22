cmake_minimum_required(VERSION 4.0)
project(compy LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
file(GLOB_RECURSE COMPY_FILES compy/*.cpp)
file(GLOB_RECURSE LIB_FILES libs/*.cpp)

add_library(compy_common STATIC ${SRC_FILES} ${COMPY_FILES} ${LIB_FILES})
target_include_directories(compy_common PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/compy ${CMAKE_SOURCE_DIR}/libs)

add_executable(compyDefaultExeName main.cpp)
target_link_libraries(compyDefaultExeName PRIVATE compy_common)

add_executable(test1 test.cpp)
target_link_libraries(test1 PRIVATE compy_common)

add_subdirectory(app_benchmarking)
add_subdirectory(app_optional)
add_subdirectory(app_text_io)
add_subdirectory(app_random_tests)