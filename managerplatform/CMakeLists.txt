cmake_minimum_required(VERSION 3.5)

project(managerplatform LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

aux_source_directory(../src CS_src)

add_executable(managerplatform
  main.cpp
  dialog.cpp
  dialog.h
  dialog.ui
  ${CS_sr}
)

target_link_libraries(managerplatform PRIVATE Qt5::Widgets)
