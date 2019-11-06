cmake_minimum_required(VERSION 3.5)

project(managerplatform LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(ManagerPlatform
  main.cpp
  dialog.cpp
  dialog.h
  dialog.ui
  client_thread.h
  client_thread.cpp
  ../src/client.h
  ../src/client.cpp
)

target_link_libraries(ManagerPlatform PRIVATE Qt5::Widgets)
