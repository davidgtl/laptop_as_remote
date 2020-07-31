cmake_minimum_required(VERSION 3.16)
project(laptop_remote)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(Boost 1.71.0 REQUIRED system program_options)

add_executable(laptop_remote src/main.cpp src/network.cpp src/network.h src/input/input_driver.cpp src/input/input_driver.h src/input/devices_descriptors.h src/util.h src/util/devicewatcher.cpp src/util/devicewatcher.h)

target_link_libraries(laptop_remote ${Boost_LIBRARIES})
target_link_libraries(laptop_remote rt)
