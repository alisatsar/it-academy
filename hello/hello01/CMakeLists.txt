cmake_minimum_required(VERSION 2.8)
add_executable(main main.cpp)
include_directories(../lib)
target_link_libraries(main greetings)
