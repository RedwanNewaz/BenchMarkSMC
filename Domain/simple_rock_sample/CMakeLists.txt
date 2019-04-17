cmake_minimum_required(VERSION 2.8.3)

add_executable("${PROJECT_NAME}_simple_rock_sample"
  src/simple_rock_sample.cpp
  src/main.cpp
)
target_link_libraries("${PROJECT_NAME}_simple_rock_sample"
  despot
)
