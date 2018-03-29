cmake_minimum_required(VERSION 2.9)
project(lib)
#к проекту присоединяются два проекта нижнего уровня
#задающих пути к подкаталогам
add_subdirectory(sample_lib)
add_subdirectory(sample_program)
