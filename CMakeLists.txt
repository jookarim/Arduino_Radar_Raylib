cmake_minimum_required(VERSION 3.20)

project(Arduino_radar)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(raylib CONFIG REQUIRED)

add_executable(Arduino_radar
    src/main.cpp
)

target_link_libraries(Arduino_radar
    PRIVATE
    raylib
)