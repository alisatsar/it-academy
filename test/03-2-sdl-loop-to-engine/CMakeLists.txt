cmake_minimum_required(VERSION 3.9)
project(03-2-sdl-loop-to-engine)

add_executable(${PROJECT_NAME} main.cxx engine.cxx engine.hxx)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

find_library(SDL2_LIB libSDL2-2.0d.so)

target_link_libraries(${PROJECT_NAME}
	-pthread
	${SDL2_LIB}
	-dl)
