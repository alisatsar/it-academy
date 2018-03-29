cmake_minimum_required(VERSION 2.9)
#это имя быдут ислозовано при создании системы
#построения в качестве имени решения или основного проекта
project(main)
#определяет новую цель построения
add_executable(main main.cxx)
include_directories(../hello_lib)
target_link_libraries(main greetings)
