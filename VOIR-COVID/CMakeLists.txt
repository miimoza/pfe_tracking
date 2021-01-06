# cmake
cmake_minimum_required(VERSION 2.8)

# project
set(TARGET_NAME covid)
project(${TARGET_NAME})

# compilations
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -std=c++17")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_STANDARD 17)

# sources
include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src
)
add_executable(${TARGET_NAME}
    src/main.cc
    src/control.cc
    src/detection.cc
)

# libs
find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui)
target_link_libraries(${TARGET_NAME} ${OpenCV_LIBS})
