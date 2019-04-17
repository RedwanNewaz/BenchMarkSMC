cmake_minimum_required(VERSION 3.13)
project(BenchMarkSMC)

set(CMAKE_CXX_STANDARD 17)

find_package(Despot CONFIG REQUIRED)

include_directories(
        include
        /usr/local/include
        Domain/rock_sample

)


set(ROCK_SAMPLE_FILES
        Domain/rock_sample/base/base_rock_sample.h
        Domain/rock_sample/rock_sample.h
        Domain/rock_sample/base/base_rock_sample.cpp
        Domain/rock_sample/rock_sample.cpp
        include/SharedData.h
        )




set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
set(HEADER_FILE include/despot/Setup.h)
set(SOURCE_FILE main.cpp Planner/despotPlanner.cpp include/Planner/despotPlanner.h)
add_subdirectory(Domain/navigation)

add_subdirectory(Domain/tag)
add_subdirectory(Domain/simple_rock_sample)



# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)

find_package(Qt5Core REQUIRED)

find_package(Qt5PrintSupport REQUIRED)



set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_RELEASE} -fprofile-arcs -ftest-coverage")

# set up a mapping so that the Release configuration for the Qt imported target is
# used in the COVERAGE CMake configuration.
set_target_properties(Qt5::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")

# Populate a CMake variable with the sources
set(QT_SOURCE_FILES
       config/QtEnv/dialog.cpp
       config/QtEnv/qcustomplot.cpp
       config/QtEnv/dialog.ui
       include/dialog.h
       include/qcustomplot.h
       config/Util/DisplayBase.cpp
       include/Util/DisplayBase.h

        ${ROCK_SAMPLE_FILES}

        include/SharedData.h)




add_executable(BenchMarkSMC ${SOURCE_FILE} ${HEADER_FILE} ${QT_SOURCE_FILES})
target_link_libraries( BenchMarkSMC
        Qt5::Widgets
        Qt5::PrintSupport
        despot)