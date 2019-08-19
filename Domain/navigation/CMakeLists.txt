cmake_minimum_required(VERSION 2.8.3)

include_directories(
        ${QT_DISP_INCLUDES}
        src
)



add_executable("${PROJECT_NAME}_navigation"
  src/navigation.cpp
  src/main.cpp
  ${QT_SOURCE_FILES} ${QT_INCLUDE_FILES}
)
target_link_libraries("${PROJECT_NAME}_navigation"
        QT_DISP_LIBS
        despot
)
