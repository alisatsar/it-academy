cmake_minimum_required(VERSION 3.9)
project(08-1-prepare-engine-for-game)

add_library(engine-08-1 SHARED engine.cxx engine.hxx picopng.hxx sdl2_opengl.hxx)
target_compile_features(engine-08-1 PUBLIC cxx_std_17)

add_library(e_math SHARED e_math.cxx e_math.hxx)
target_compile_features(e_math PUBLIC cxx_std_17)

add_library(color SHARED color.cxx color.hxx)
target_compile_features(color PUBLIC cxx_std_17)

if(WIN32)
  target_compile_definitions(engine-08-1 PRIVATE "-DOM_DECLSPEC=__declspec(dllexport)")
endif(WIN32)

find_library(SDL2_LIB NAMES SDL2)

if (MINGW)
    target_link_libraries(engine-08-1
    			e_math
    			color    			
               -lmingw32
               -lSDL2main
               -lSDL2
               -mwindows
               -lopengl32
               )
elseif(UNIX)
    target_link_libraries(engine-08-1
    			e_math
    			color
               -lSDL2
               -lGL
               )
elseif(MSVC)
    find_package(sdl2 REQUIRED)
    target_link_libraries(engine-08-1 PRIVATE SDL2::SDL2 SDL2::SDL2main
                          opengl32)
endif()

add_executable(game-08-1 game.cxx)
target_compile_features(game-08-1 PUBLIC cxx_std_17)

target_link_libraries(game-08-1 engine-08-1 e_math color)

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  # using regular Clang or AppleClang
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror")
endif()
