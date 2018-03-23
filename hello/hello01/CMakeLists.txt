cmake_minimum_required(VERSION 2.8)
#определяет новую цель построения с именем main и исходным файлом main.cpp
add_executable(main main.cpp)
include_directories(../lib)
target_link_libraries(main greetings)
