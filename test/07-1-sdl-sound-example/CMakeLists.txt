cmake_minimum_required(VERSION 3.9)
project(07_audio)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror")

find_library(SDL2_LIB libSDL2-2.0d.so)

add_executable(sound_test-07-1 audio.c main.c audio.h)

target_link_libraries(sound_test-07-1
	-pthread
	${SDL2_LIB}
	-lGL)	
