cmake_minimum_required(VERSION 3.9)
project(02-1-sdl-static)

add_executable(${PROJECT_NAME} main.cxx)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

#sdl2-confit -static--libs
#используется для всего, а нам нужны только либы
target_link_libraries(${PROJECT_NAME}
	-lm
	#-l говорит, линкуйся с библиотекой
	#-L обозначает путь для include directories
	-pthread
	libSDL2d.a
	#приходится так явно писать для статической библиотеки
	-ldl)