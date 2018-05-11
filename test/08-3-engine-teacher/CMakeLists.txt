cmake_minimum_required(VERSION 3.9)
project(game)

add_library(engine SHARED engine.cxx e_math.hxx)
target_compile_features(engine PUBLIC cxx_std_17)

add_library(color SHARED color.cxx)
target_compile_features(color PUBLIC cxx_std_17)

find_library(SDL2_LIB NAMES SDL2)

target_link_libraries(engine
	color
	-pthread
	-lSDL2
	-lGL)
	
add_executable(game game.cxx)
target_compile_features(game PUBLIC cxx_std_17)

target_link_libraries(game engine color)