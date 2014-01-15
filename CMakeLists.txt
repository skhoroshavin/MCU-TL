project(AVRTest)
cmake_minimum_required(VERSION 2.8)
add_definitions(-Os -std=c++11)

include_directories(mcutl mcutl/AVR)

aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

