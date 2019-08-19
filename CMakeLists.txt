cmake_minimum_required(VERSION 3.13)
project(BenchMarkSMC)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

find_package(Despot CONFIG REQUIRED)

include_directories(
        include
        /usr/local/include
)

#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
set(HEADER_FILE include/despot/Setup.h)
set(SOURCE_FILE main.cpp Planner/despotPlanner.cpp include/Planner/despotPlanner.h)

find_package(QT)
add_subdirectory(Domain/tag)
add_subdirectory(Domain/navigation)
add_subdirectory(Domain/rock_sample)
