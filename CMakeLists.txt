cmake_minimum_required(VERSION 2.8.12)
project(opencv_stitcher)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(stitcher stitcher.cpp)
target_link_libraries(stitcher ${CONAN_LIBS})