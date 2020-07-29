cmake_minimum_required(VERSION 3.17)
project(laptop_remote)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(Boost 1.71.0 REQUIRED system program_options)

add_executable(laptop_remote main.cpp)

target_link_libraries(laptop_remote ${Boost_LIBRARIES})
