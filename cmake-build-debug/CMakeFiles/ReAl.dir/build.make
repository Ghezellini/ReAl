# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.20

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2021.2.3\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2021.2.3\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\GhezelsofluMR\CLionProjects\ReAl

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\GhezelsofluMR\CLionProjects\ReAl\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/ReAl.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/ReAl.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ReAl.dir/flags.make

CMakeFiles/ReAl.dir/main.cpp.obj: CMakeFiles/ReAl.dir/flags.make
CMakeFiles/ReAl.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\GhezelsofluMR\CLionProjects\ReAl\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ReAl.dir/main.cpp.obj"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\ReAl.dir\main.cpp.obj -c C:\Users\GhezelsofluMR\CLionProjects\ReAl\main.cpp

CMakeFiles/ReAl.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ReAl.dir/main.cpp.i"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\GhezelsofluMR\CLionProjects\ReAl\main.cpp > CMakeFiles\ReAl.dir\main.cpp.i

CMakeFiles/ReAl.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ReAl.dir/main.cpp.s"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\GhezelsofluMR\CLionProjects\ReAl\main.cpp -o CMakeFiles\ReAl.dir\main.cpp.s

# Object files for target ReAl
ReAl_OBJECTS = \
"CMakeFiles/ReAl.dir/main.cpp.obj"

# External object files for target ReAl
ReAl_EXTERNAL_OBJECTS =

ReAl.exe: CMakeFiles/ReAl.dir/main.cpp.obj
ReAl.exe: CMakeFiles/ReAl.dir/build.make
ReAl.exe: CMakeFiles/ReAl.dir/linklibs.rsp
ReAl.exe: CMakeFiles/ReAl.dir/objects1.rsp
ReAl.exe: CMakeFiles/ReAl.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\GhezelsofluMR\CLionProjects\ReAl\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ReAl.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\ReAl.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ReAl.dir/build: ReAl.exe
.PHONY : CMakeFiles/ReAl.dir/build

CMakeFiles/ReAl.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\ReAl.dir\cmake_clean.cmake
.PHONY : CMakeFiles/ReAl.dir/clean

CMakeFiles/ReAl.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\GhezelsofluMR\CLionProjects\ReAl C:\Users\GhezelsofluMR\CLionProjects\ReAl C:\Users\GhezelsofluMR\CLionProjects\ReAl\cmake-build-debug C:\Users\GhezelsofluMR\CLionProjects\ReAl\cmake-build-debug C:\Users\GhezelsofluMR\CLionProjects\ReAl\cmake-build-debug\CMakeFiles\ReAl.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ReAl.dir/depend
