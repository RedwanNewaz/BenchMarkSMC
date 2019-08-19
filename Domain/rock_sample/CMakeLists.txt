cmake_minimum_required(VERSION 2.8.3)

include_directories(
    src
    ${QT_DISP_INCLUDES}
)

add_executable("${PROJECT_NAME}_rock_sample"
  src/base/base_rock_sample.cpp
  src/rock_sample/rock_sample.cpp
  src/rock_sample/main.cpp
  ${QT_SOURCE_FILES} ${QT_INCLUDE_FILES}
)
target_link_libraries("${PROJECT_NAME}_rock_sample"
        QT_DISP_LIBS
        despot
        )




add_executable("${PROJECT_NAME}_fvrs"
  src/base/base_rock_sample.cpp
  src/fvrs/fvrs.cpp
  src/fvrs/main.cpp
)
target_link_libraries("${PROJECT_NAME}_fvrs"

  despot
)

