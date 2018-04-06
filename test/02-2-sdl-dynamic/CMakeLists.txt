cmake_minimum_required(VERSION 3.9)
project(02-2-sdl-dynamic)

add_executable(${PROJECT_NAME} main.cxx)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

find_library(SDL2_LIB libSDL2-2.0d.so)
 #HINTS "/usr/local/lib")

message(${SDL2_LIB})
#sdl2-confit -static--libs
#используется для всего, а нам нужны только либы
target_link_libraries(${PROJECT_NAME}
	#-l говорит, линкуйся с библиотекой
	#-L обозначает путь для include directories
	-pthread
	${SDL2_LIB}
	#приходится так явно писать для статической библиотеки
	dl)