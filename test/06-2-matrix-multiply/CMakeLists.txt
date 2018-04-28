cmake_minimum_required(VERSION 3.9)
project(06_matrix)

add_library(engine SHARED engine.cxx)
target_compile_features(engine PUBLIC cxx_std_17)
add_library(my_math SHARED my_math.cxx)
target_compile_features(my_math PUBLIC cxx_std_17)

find_library(SDL2_LIB libSDL2-2.0d.so)

target_link_libraries(engine
	my_math	
	-pthread
	${SDL2_LIB}
	-dl
	-lGLESv2
	-lglfw)
	
add_executable(game-03-3 main.cxx)
target_compile_features(game-03-3 PUBLIC cxx_std_17)

target_link_libraries(game-03-3 engine my_math)
