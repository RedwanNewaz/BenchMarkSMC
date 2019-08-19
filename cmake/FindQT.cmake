
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

set(QT_DISP_INCLUDES ${PROJECT_SOURCE_DIR}/include)
# Populate a CMake variable with the sources
set(QT_INCLUDE_FILES
        ${PROJECT_SOURCE_DIR}/include/dialog.h
        ${PROJECT_SOURCE_DIR}/include/qcustomplot.h
        ${PROJECT_SOURCE_DIR}/include/Util/DisplayBase.h
        ${PROJECT_SOURCE_DIR}/include/SharedData.h

        )

set(QT_SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/config/QtEnv/dialog.cpp
        ${PROJECT_SOURCE_DIR}/config/QtEnv/qcustomplot.cpp
        ${PROJECT_SOURCE_DIR}/config/QtEnv/dialog.ui
        ${PROJECT_SOURCE_DIR}/config/Util/DisplayBase.cpp
        )

add_library(QT_DISP_LIBS ${QT_SOURCE_FILES} ${QT_INCLUDE_FILES})

target_link_libraries(QT_DISP_LIBS Qt5::Widgets Qt5::PrintSupport)