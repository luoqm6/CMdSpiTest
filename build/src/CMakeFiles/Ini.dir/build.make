# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/luoqingming/coding/CMdSpiTest

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/luoqingming/coding/CMdSpiTest/build

# Include any dependencies generated for this target.
include src/CMakeFiles/Ini.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/Ini.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/Ini.dir/flags.make

src/CMakeFiles/Ini.dir/Ini.cpp.o: src/CMakeFiles/Ini.dir/flags.make
src/CMakeFiles/Ini.dir/Ini.cpp.o: ../src/Ini.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/luoqingming/coding/CMdSpiTest/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/Ini.dir/Ini.cpp.o"
	cd /home/luoqingming/coding/CMdSpiTest/build/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Ini.dir/Ini.cpp.o -c /home/luoqingming/coding/CMdSpiTest/src/Ini.cpp

src/CMakeFiles/Ini.dir/Ini.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Ini.dir/Ini.cpp.i"
	cd /home/luoqingming/coding/CMdSpiTest/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/luoqingming/coding/CMdSpiTest/src/Ini.cpp > CMakeFiles/Ini.dir/Ini.cpp.i

src/CMakeFiles/Ini.dir/Ini.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Ini.dir/Ini.cpp.s"
	cd /home/luoqingming/coding/CMdSpiTest/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/luoqingming/coding/CMdSpiTest/src/Ini.cpp -o CMakeFiles/Ini.dir/Ini.cpp.s

src/CMakeFiles/Ini.dir/Ini.cpp.o.requires:

.PHONY : src/CMakeFiles/Ini.dir/Ini.cpp.o.requires

src/CMakeFiles/Ini.dir/Ini.cpp.o.provides: src/CMakeFiles/Ini.dir/Ini.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/Ini.dir/build.make src/CMakeFiles/Ini.dir/Ini.cpp.o.provides.build
.PHONY : src/CMakeFiles/Ini.dir/Ini.cpp.o.provides

src/CMakeFiles/Ini.dir/Ini.cpp.o.provides.build: src/CMakeFiles/Ini.dir/Ini.cpp.o


# Object files for target Ini
Ini_OBJECTS = \
"CMakeFiles/Ini.dir/Ini.cpp.o"

# External object files for target Ini
Ini_EXTERNAL_OBJECTS =

../lib/libIni.so: src/CMakeFiles/Ini.dir/Ini.cpp.o
../lib/libIni.so: src/CMakeFiles/Ini.dir/build.make
../lib/libIni.so: src/CMakeFiles/Ini.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/luoqingming/coding/CMdSpiTest/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library ../../lib/libIni.so"
	cd /home/luoqingming/coding/CMdSpiTest/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Ini.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/Ini.dir/build: ../lib/libIni.so

.PHONY : src/CMakeFiles/Ini.dir/build

src/CMakeFiles/Ini.dir/requires: src/CMakeFiles/Ini.dir/Ini.cpp.o.requires

.PHONY : src/CMakeFiles/Ini.dir/requires

src/CMakeFiles/Ini.dir/clean:
	cd /home/luoqingming/coding/CMdSpiTest/build/src && $(CMAKE_COMMAND) -P CMakeFiles/Ini.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/Ini.dir/clean

src/CMakeFiles/Ini.dir/depend:
	cd /home/luoqingming/coding/CMdSpiTest/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/luoqingming/coding/CMdSpiTest /home/luoqingming/coding/CMdSpiTest/src /home/luoqingming/coding/CMdSpiTest/build /home/luoqingming/coding/CMdSpiTest/build/src /home/luoqingming/coding/CMdSpiTest/build/src/CMakeFiles/Ini.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/Ini.dir/depend

