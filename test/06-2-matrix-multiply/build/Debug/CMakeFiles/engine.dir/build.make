# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/alisatsar/Desktop/online-lessons/06-3-test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/alisatsar/Desktop/online-lessons/06-3-test/build/Debug

# Include any dependencies generated for this target.
include CMakeFiles/engine.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/engine.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/engine.dir/flags.make

CMakeFiles/engine.dir/engine.cxx.o: CMakeFiles/engine.dir/flags.make
CMakeFiles/engine.dir/engine.cxx.o: ../../engine.cxx
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alisatsar/Desktop/online-lessons/06-3-test/build/Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/engine.dir/engine.cxx.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/engine.dir/engine.cxx.o -c /home/alisatsar/Desktop/online-lessons/06-3-test/engine.cxx

CMakeFiles/engine.dir/engine.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/engine.dir/engine.cxx.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/alisatsar/Desktop/online-lessons/06-3-test/engine.cxx > CMakeFiles/engine.dir/engine.cxx.i

CMakeFiles/engine.dir/engine.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/engine.dir/engine.cxx.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/alisatsar/Desktop/online-lessons/06-3-test/engine.cxx -o CMakeFiles/engine.dir/engine.cxx.s

CMakeFiles/engine.dir/engine.cxx.o.requires:

.PHONY : CMakeFiles/engine.dir/engine.cxx.o.requires

CMakeFiles/engine.dir/engine.cxx.o.provides: CMakeFiles/engine.dir/engine.cxx.o.requires
	$(MAKE) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/engine.cxx.o.provides.build
.PHONY : CMakeFiles/engine.dir/engine.cxx.o.provides

CMakeFiles/engine.dir/engine.cxx.o.provides.build: CMakeFiles/engine.dir/engine.cxx.o


# Object files for target engine
engine_OBJECTS = \
"CMakeFiles/engine.dir/engine.cxx.o"

# External object files for target engine
engine_EXTERNAL_OBJECTS =

libengine.so: CMakeFiles/engine.dir/engine.cxx.o
libengine.so: CMakeFiles/engine.dir/build.make
libengine.so: libmy_math.so
libengine.so: /usr/local/lib/libSDL2-2.0d.so
libengine.so: CMakeFiles/engine.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/alisatsar/Desktop/online-lessons/06-3-test/build/Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libengine.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/engine.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/engine.dir/build: libengine.so

.PHONY : CMakeFiles/engine.dir/build

CMakeFiles/engine.dir/requires: CMakeFiles/engine.dir/engine.cxx.o.requires

.PHONY : CMakeFiles/engine.dir/requires

CMakeFiles/engine.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/engine.dir/cmake_clean.cmake
.PHONY : CMakeFiles/engine.dir/clean

CMakeFiles/engine.dir/depend:
	cd /home/alisatsar/Desktop/online-lessons/06-3-test/build/Debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/alisatsar/Desktop/online-lessons/06-3-test /home/alisatsar/Desktop/online-lessons/06-3-test /home/alisatsar/Desktop/online-lessons/06-3-test/build/Debug /home/alisatsar/Desktop/online-lessons/06-3-test/build/Debug /home/alisatsar/Desktop/online-lessons/06-3-test/build/Debug/CMakeFiles/engine.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/engine.dir/depend
