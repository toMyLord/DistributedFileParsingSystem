cmake_minimum_required(VERSION 3.10)
project(DistributedFileParsing)

set(CMAKE_CXX_STANDARD 11)

aux_source_directory(fileparsing/src FileParsing_src)
aux_source_directory(taskdistribution/src TaskDistribution_src)
aux_source_directory(workstation/src WorkStation_src)

add_executable(FileParsing ./fileparsing/FileParsing_main.cpp ${FileParsing_src})
add_executable(TaskDistribution ./taskdistribution/TaskDistribution_main.cpp ${TaskDistribution_src})
add_executable(WorkStation ./workstation/WorkStation_main.cpp ${WorkStation_src})