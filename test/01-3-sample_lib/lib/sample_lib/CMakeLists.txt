cmake_minimum_required(VERSION 2.9)
#задаем имя цели и список исходных файлов проекта
#сборка приведет к созданию статической библиотеки
#имя для компилятора будет libsample_lib.a
add_library(sample_lib sample_module.cxx sample_module.hpp)
