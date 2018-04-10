cmake_minimum_required(VERSION 3.9)
project(03-1-sdl-loop)

add_executable(${PROJECT_NAME} main.cxx)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

find_library(SDL2_LIB libSDL2-2.0d.so)
 #HINTS "/usr/local/lib")

message(${SDL2_LIB})
target_link_libraries(${PROJECT_NAME}
	-pthread
	${SDL2_LIB}
	dl)