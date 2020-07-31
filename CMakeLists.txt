cmake_minimum_required(VERSION 3.16)
project(laptop_remote)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

file(GLOB_RECURSE SOURCES "src/*.h" "src/*.cpp")

find_package(Boost 1.71.0 REQUIRED system program_options)

add_executable(${PROJECT_NAME} ${SOURCES})

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
# set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libraries")
include_directories("${SRC_DIR}")

target_link_libraries(${PROJECT_NAME}  ${Boost_LIBRARIES})
target_link_libraries(${PROJECT_NAME}  rt)
