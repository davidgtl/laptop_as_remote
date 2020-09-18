cmake_minimum_required(VERSION 3.5)
project(laptop_remote)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

file(GLOB_RECURSE SOURCES "src/*.h" "src/*.cpp" "src/*.hpp")

find_package(Boost 1.71.0 REQUIRED system program_options)

add_executable(${PROJECT_NAME} ${SOURCES})

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs")
include_directories(${SRC_DIR} ${LIB_DIR})


add_subdirectory(${LIB_DIR}/tink)

target_link_libraries(${PROJECT_NAME}  tink::static)

target_link_libraries(${PROJECT_NAME}  ${Boost_LIBRARIES})
target_link_libraries(${PROJECT_NAME}  rt)
