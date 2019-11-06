cmake_minimum_required(VERSION 3.10)
project(DistributedFileParsingSystem)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
# 开启Qt代码自动生成，不再需用自己手写了。按顺序，分别是`Q_OBJECT`宏展开，资源文件，界面文件。
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMKAE_THREAD_PREFER_PTHREAD)


aux_source_directory(fileparsing/src FileParsing_src)
aux_source_directory(taskdistribution/src TaskDistribution_src)
aux_source_directory(workstation/src WorkStation_src)
aux_source_directory(managerplatform ManagerPlatform_src)
aux_source_directory(src CS_src)


add_executable(FileParsing
        ./fileparsing/FileParsing_main.cpp
        ${FileParsing_src}
        ${CS_src})
add_executable(TaskDistribution
        ./taskdistribution/TaskDistribution_main.cpp
        ${TaskDistribution_src}
        ${CS_src})
add_executable(WorkStation
        ./workstation/WorkStation_main.cpp
        ${WorkStation_src}
        ${CS_src})
add_executable(ManagerPlatform
        ${ManagerPlatform_src}
        ${CS_src}
        )

target_link_libraries(TaskDistribution pthread)
target_link_libraries(ManagerPlatform PRIVATE Qt5::Widgets)