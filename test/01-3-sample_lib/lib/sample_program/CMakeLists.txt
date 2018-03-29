cmake_minimum_required(VERSION 2.9)
project(sample_program)
add_executable(sample_program main.cxx)
#определяет дополнительные пути поиска заг файлов компилятором
#gcc ... -I <path> ...
include_directories(../sample_lib)
#определяет зависимость цели sample_program от sample_lib
#при построении цели sample_program будет использоваться библиотека 
#создаваемая в результате построения цели sample_lib
target_link_libraries(sample_program sample_lib)
