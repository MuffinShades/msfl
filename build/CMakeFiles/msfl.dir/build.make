# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/jamesweigand/Documents/msfl

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/jamesweigand/Documents/msfl/build

# Include any dependencies generated for this target.
include CMakeFiles/msfl.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/msfl.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/msfl.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/msfl.dir/flags.make

CMakeFiles/msfl.dir/src/BinUtility.cpp.o: CMakeFiles/msfl.dir/flags.make
CMakeFiles/msfl.dir/src/BinUtility.cpp.o: /Users/jamesweigand/Documents/msfl/src/BinUtility.cpp
CMakeFiles/msfl.dir/src/BinUtility.cpp.o: CMakeFiles/msfl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/jamesweigand/Documents/msfl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/msfl.dir/src/BinUtility.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/msfl.dir/src/BinUtility.cpp.o -MF CMakeFiles/msfl.dir/src/BinUtility.cpp.o.d -o CMakeFiles/msfl.dir/src/BinUtility.cpp.o -c /Users/jamesweigand/Documents/msfl/src/BinUtility.cpp

CMakeFiles/msfl.dir/src/BinUtility.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/msfl.dir/src/BinUtility.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jamesweigand/Documents/msfl/src/BinUtility.cpp > CMakeFiles/msfl.dir/src/BinUtility.cpp.i

CMakeFiles/msfl.dir/src/BinUtility.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/msfl.dir/src/BinUtility.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jamesweigand/Documents/msfl/src/BinUtility.cpp -o CMakeFiles/msfl.dir/src/BinUtility.cpp.s

CMakeFiles/msfl.dir/src/ByteStream.cpp.o: CMakeFiles/msfl.dir/flags.make
CMakeFiles/msfl.dir/src/ByteStream.cpp.o: /Users/jamesweigand/Documents/msfl/src/ByteStream.cpp
CMakeFiles/msfl.dir/src/ByteStream.cpp.o: CMakeFiles/msfl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/jamesweigand/Documents/msfl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/msfl.dir/src/ByteStream.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/msfl.dir/src/ByteStream.cpp.o -MF CMakeFiles/msfl.dir/src/ByteStream.cpp.o.d -o CMakeFiles/msfl.dir/src/ByteStream.cpp.o -c /Users/jamesweigand/Documents/msfl/src/ByteStream.cpp

CMakeFiles/msfl.dir/src/ByteStream.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/msfl.dir/src/ByteStream.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jamesweigand/Documents/msfl/src/ByteStream.cpp > CMakeFiles/msfl.dir/src/ByteStream.cpp.i

CMakeFiles/msfl.dir/src/ByteStream.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/msfl.dir/src/ByteStream.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jamesweigand/Documents/msfl/src/ByteStream.cpp -o CMakeFiles/msfl.dir/src/ByteStream.cpp.s

CMakeFiles/msfl.dir/src/FileWriter.cpp.o: CMakeFiles/msfl.dir/flags.make
CMakeFiles/msfl.dir/src/FileWriter.cpp.o: /Users/jamesweigand/Documents/msfl/src/FileWriter.cpp
CMakeFiles/msfl.dir/src/FileWriter.cpp.o: CMakeFiles/msfl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/jamesweigand/Documents/msfl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/msfl.dir/src/FileWriter.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/msfl.dir/src/FileWriter.cpp.o -MF CMakeFiles/msfl.dir/src/FileWriter.cpp.o.d -o CMakeFiles/msfl.dir/src/FileWriter.cpp.o -c /Users/jamesweigand/Documents/msfl/src/FileWriter.cpp

CMakeFiles/msfl.dir/src/FileWriter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/msfl.dir/src/FileWriter.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jamesweigand/Documents/msfl/src/FileWriter.cpp > CMakeFiles/msfl.dir/src/FileWriter.cpp.i

CMakeFiles/msfl.dir/src/FileWriter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/msfl.dir/src/FileWriter.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jamesweigand/Documents/msfl/src/FileWriter.cpp -o CMakeFiles/msfl.dir/src/FileWriter.cpp.s

CMakeFiles/msfl.dir/src/RawBytes.cpp.o: CMakeFiles/msfl.dir/flags.make
CMakeFiles/msfl.dir/src/RawBytes.cpp.o: /Users/jamesweigand/Documents/msfl/src/RawBytes.cpp
CMakeFiles/msfl.dir/src/RawBytes.cpp.o: CMakeFiles/msfl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/jamesweigand/Documents/msfl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/msfl.dir/src/RawBytes.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/msfl.dir/src/RawBytes.cpp.o -MF CMakeFiles/msfl.dir/src/RawBytes.cpp.o.d -o CMakeFiles/msfl.dir/src/RawBytes.cpp.o -c /Users/jamesweigand/Documents/msfl/src/RawBytes.cpp

CMakeFiles/msfl.dir/src/RawBytes.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/msfl.dir/src/RawBytes.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jamesweigand/Documents/msfl/src/RawBytes.cpp > CMakeFiles/msfl.dir/src/RawBytes.cpp.i

CMakeFiles/msfl.dir/src/RawBytes.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/msfl.dir/src/RawBytes.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jamesweigand/Documents/msfl/src/RawBytes.cpp -o CMakeFiles/msfl.dir/src/RawBytes.cpp.s

CMakeFiles/msfl.dir/src/asset.cpp.o: CMakeFiles/msfl.dir/flags.make
CMakeFiles/msfl.dir/src/asset.cpp.o: /Users/jamesweigand/Documents/msfl/src/asset.cpp
CMakeFiles/msfl.dir/src/asset.cpp.o: CMakeFiles/msfl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/jamesweigand/Documents/msfl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/msfl.dir/src/asset.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/msfl.dir/src/asset.cpp.o -MF CMakeFiles/msfl.dir/src/asset.cpp.o.d -o CMakeFiles/msfl.dir/src/asset.cpp.o -c /Users/jamesweigand/Documents/msfl/src/asset.cpp

CMakeFiles/msfl.dir/src/asset.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/msfl.dir/src/asset.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jamesweigand/Documents/msfl/src/asset.cpp > CMakeFiles/msfl.dir/src/asset.cpp.i

CMakeFiles/msfl.dir/src/asset.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/msfl.dir/src/asset.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jamesweigand/Documents/msfl/src/asset.cpp -o CMakeFiles/msfl.dir/src/asset.cpp.s

CMakeFiles/msfl.dir/src/balloon.cpp.o: CMakeFiles/msfl.dir/flags.make
CMakeFiles/msfl.dir/src/balloon.cpp.o: /Users/jamesweigand/Documents/msfl/src/balloon.cpp
CMakeFiles/msfl.dir/src/balloon.cpp.o: CMakeFiles/msfl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/jamesweigand/Documents/msfl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/msfl.dir/src/balloon.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/msfl.dir/src/balloon.cpp.o -MF CMakeFiles/msfl.dir/src/balloon.cpp.o.d -o CMakeFiles/msfl.dir/src/balloon.cpp.o -c /Users/jamesweigand/Documents/msfl/src/balloon.cpp

CMakeFiles/msfl.dir/src/balloon.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/msfl.dir/src/balloon.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jamesweigand/Documents/msfl/src/balloon.cpp > CMakeFiles/msfl.dir/src/balloon.cpp.i

CMakeFiles/msfl.dir/src/balloon.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/msfl.dir/src/balloon.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jamesweigand/Documents/msfl/src/balloon.cpp -o CMakeFiles/msfl.dir/src/balloon.cpp.s

CMakeFiles/msfl.dir/src/bitmap.cpp.o: CMakeFiles/msfl.dir/flags.make
CMakeFiles/msfl.dir/src/bitmap.cpp.o: /Users/jamesweigand/Documents/msfl/src/bitmap.cpp
CMakeFiles/msfl.dir/src/bitmap.cpp.o: CMakeFiles/msfl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/jamesweigand/Documents/msfl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/msfl.dir/src/bitmap.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/msfl.dir/src/bitmap.cpp.o -MF CMakeFiles/msfl.dir/src/bitmap.cpp.o.d -o CMakeFiles/msfl.dir/src/bitmap.cpp.o -c /Users/jamesweigand/Documents/msfl/src/bitmap.cpp

CMakeFiles/msfl.dir/src/bitmap.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/msfl.dir/src/bitmap.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jamesweigand/Documents/msfl/src/bitmap.cpp > CMakeFiles/msfl.dir/src/bitmap.cpp.i

CMakeFiles/msfl.dir/src/bitmap.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/msfl.dir/src/bitmap.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jamesweigand/Documents/msfl/src/bitmap.cpp -o CMakeFiles/msfl.dir/src/bitmap.cpp.s

CMakeFiles/msfl.dir/src/bitmapRender.cpp.o: CMakeFiles/msfl.dir/flags.make
CMakeFiles/msfl.dir/src/bitmapRender.cpp.o: /Users/jamesweigand/Documents/msfl/src/bitmapRender.cpp
CMakeFiles/msfl.dir/src/bitmapRender.cpp.o: CMakeFiles/msfl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/jamesweigand/Documents/msfl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/msfl.dir/src/bitmapRender.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/msfl.dir/src/bitmapRender.cpp.o -MF CMakeFiles/msfl.dir/src/bitmapRender.cpp.o.d -o CMakeFiles/msfl.dir/src/bitmapRender.cpp.o -c /Users/jamesweigand/Documents/msfl/src/bitmapRender.cpp

CMakeFiles/msfl.dir/src/bitmapRender.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/msfl.dir/src/bitmapRender.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jamesweigand/Documents/msfl/src/bitmapRender.cpp > CMakeFiles/msfl.dir/src/bitmapRender.cpp.i

CMakeFiles/msfl.dir/src/bitmapRender.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/msfl.dir/src/bitmapRender.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jamesweigand/Documents/msfl/src/bitmapRender.cpp -o CMakeFiles/msfl.dir/src/bitmapRender.cpp.s

CMakeFiles/msfl.dir/src/cfgParse.cpp.o: CMakeFiles/msfl.dir/flags.make
CMakeFiles/msfl.dir/src/cfgParse.cpp.o: /Users/jamesweigand/Documents/msfl/src/cfgParse.cpp
CMakeFiles/msfl.dir/src/cfgParse.cpp.o: CMakeFiles/msfl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/jamesweigand/Documents/msfl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/msfl.dir/src/cfgParse.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/msfl.dir/src/cfgParse.cpp.o -MF CMakeFiles/msfl.dir/src/cfgParse.cpp.o.d -o CMakeFiles/msfl.dir/src/cfgParse.cpp.o -c /Users/jamesweigand/Documents/msfl/src/cfgParse.cpp

CMakeFiles/msfl.dir/src/cfgParse.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/msfl.dir/src/cfgParse.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jamesweigand/Documents/msfl/src/cfgParse.cpp > CMakeFiles/msfl.dir/src/cfgParse.cpp.i

CMakeFiles/msfl.dir/src/cfgParse.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/msfl.dir/src/cfgParse.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jamesweigand/Documents/msfl/src/cfgParse.cpp -o CMakeFiles/msfl.dir/src/cfgParse.cpp.s

CMakeFiles/msfl.dir/src/jparse.cpp.o: CMakeFiles/msfl.dir/flags.make
CMakeFiles/msfl.dir/src/jparse.cpp.o: /Users/jamesweigand/Documents/msfl/src/jparse.cpp
CMakeFiles/msfl.dir/src/jparse.cpp.o: CMakeFiles/msfl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/jamesweigand/Documents/msfl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/msfl.dir/src/jparse.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/msfl.dir/src/jparse.cpp.o -MF CMakeFiles/msfl.dir/src/jparse.cpp.o.d -o CMakeFiles/msfl.dir/src/jparse.cpp.o -c /Users/jamesweigand/Documents/msfl/src/jparse.cpp

CMakeFiles/msfl.dir/src/jparse.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/msfl.dir/src/jparse.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jamesweigand/Documents/msfl/src/jparse.cpp > CMakeFiles/msfl.dir/src/jparse.cpp.i

CMakeFiles/msfl.dir/src/jparse.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/msfl.dir/src/jparse.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jamesweigand/Documents/msfl/src/jparse.cpp -o CMakeFiles/msfl.dir/src/jparse.cpp.s

CMakeFiles/msfl.dir/src/main.cpp.o: CMakeFiles/msfl.dir/flags.make
CMakeFiles/msfl.dir/src/main.cpp.o: /Users/jamesweigand/Documents/msfl/src/main.cpp
CMakeFiles/msfl.dir/src/main.cpp.o: CMakeFiles/msfl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/jamesweigand/Documents/msfl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/msfl.dir/src/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/msfl.dir/src/main.cpp.o -MF CMakeFiles/msfl.dir/src/main.cpp.o.d -o CMakeFiles/msfl.dir/src/main.cpp.o -c /Users/jamesweigand/Documents/msfl/src/main.cpp

CMakeFiles/msfl.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/msfl.dir/src/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jamesweigand/Documents/msfl/src/main.cpp > CMakeFiles/msfl.dir/src/main.cpp.i

CMakeFiles/msfl.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/msfl.dir/src/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jamesweigand/Documents/msfl/src/main.cpp -o CMakeFiles/msfl.dir/src/main.cpp.s

CMakeFiles/msfl.dir/src/pak.cpp.o: CMakeFiles/msfl.dir/flags.make
CMakeFiles/msfl.dir/src/pak.cpp.o: /Users/jamesweigand/Documents/msfl/src/pak.cpp
CMakeFiles/msfl.dir/src/pak.cpp.o: CMakeFiles/msfl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/jamesweigand/Documents/msfl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/msfl.dir/src/pak.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/msfl.dir/src/pak.cpp.o -MF CMakeFiles/msfl.dir/src/pak.cpp.o.d -o CMakeFiles/msfl.dir/src/pak.cpp.o -c /Users/jamesweigand/Documents/msfl/src/pak.cpp

CMakeFiles/msfl.dir/src/pak.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/msfl.dir/src/pak.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jamesweigand/Documents/msfl/src/pak.cpp > CMakeFiles/msfl.dir/src/pak.cpp.i

CMakeFiles/msfl.dir/src/pak.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/msfl.dir/src/pak.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jamesweigand/Documents/msfl/src/pak.cpp -o CMakeFiles/msfl.dir/src/pak.cpp.s

CMakeFiles/msfl.dir/src/png.cpp.o: CMakeFiles/msfl.dir/flags.make
CMakeFiles/msfl.dir/src/png.cpp.o: /Users/jamesweigand/Documents/msfl/src/png.cpp
CMakeFiles/msfl.dir/src/png.cpp.o: CMakeFiles/msfl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/jamesweigand/Documents/msfl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/msfl.dir/src/png.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/msfl.dir/src/png.cpp.o -MF CMakeFiles/msfl.dir/src/png.cpp.o.d -o CMakeFiles/msfl.dir/src/png.cpp.o -c /Users/jamesweigand/Documents/msfl/src/png.cpp

CMakeFiles/msfl.dir/src/png.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/msfl.dir/src/png.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jamesweigand/Documents/msfl/src/png.cpp > CMakeFiles/msfl.dir/src/png.cpp.i

CMakeFiles/msfl.dir/src/png.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/msfl.dir/src/png.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jamesweigand/Documents/msfl/src/png.cpp -o CMakeFiles/msfl.dir/src/png.cpp.s

CMakeFiles/msfl.dir/src/ttf.cpp.o: CMakeFiles/msfl.dir/flags.make
CMakeFiles/msfl.dir/src/ttf.cpp.o: /Users/jamesweigand/Documents/msfl/src/ttf.cpp
CMakeFiles/msfl.dir/src/ttf.cpp.o: CMakeFiles/msfl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/jamesweigand/Documents/msfl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/msfl.dir/src/ttf.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/msfl.dir/src/ttf.cpp.o -MF CMakeFiles/msfl.dir/src/ttf.cpp.o.d -o CMakeFiles/msfl.dir/src/ttf.cpp.o -c /Users/jamesweigand/Documents/msfl/src/ttf.cpp

CMakeFiles/msfl.dir/src/ttf.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/msfl.dir/src/ttf.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jamesweigand/Documents/msfl/src/ttf.cpp > CMakeFiles/msfl.dir/src/ttf.cpp.i

CMakeFiles/msfl.dir/src/ttf.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/msfl.dir/src/ttf.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jamesweigand/Documents/msfl/src/ttf.cpp -o CMakeFiles/msfl.dir/src/ttf.cpp.s

CMakeFiles/msfl.dir/src/ttfRender.cpp.o: CMakeFiles/msfl.dir/flags.make
CMakeFiles/msfl.dir/src/ttfRender.cpp.o: /Users/jamesweigand/Documents/msfl/src/ttfRender.cpp
CMakeFiles/msfl.dir/src/ttfRender.cpp.o: CMakeFiles/msfl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/jamesweigand/Documents/msfl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building CXX object CMakeFiles/msfl.dir/src/ttfRender.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/msfl.dir/src/ttfRender.cpp.o -MF CMakeFiles/msfl.dir/src/ttfRender.cpp.o.d -o CMakeFiles/msfl.dir/src/ttfRender.cpp.o -c /Users/jamesweigand/Documents/msfl/src/ttfRender.cpp

CMakeFiles/msfl.dir/src/ttfRender.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/msfl.dir/src/ttfRender.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jamesweigand/Documents/msfl/src/ttfRender.cpp > CMakeFiles/msfl.dir/src/ttfRender.cpp.i

CMakeFiles/msfl.dir/src/ttfRender.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/msfl.dir/src/ttfRender.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jamesweigand/Documents/msfl/src/ttfRender.cpp -o CMakeFiles/msfl.dir/src/ttfRender.cpp.s

CMakeFiles/msfl.dir/src/xml.cpp.o: CMakeFiles/msfl.dir/flags.make
CMakeFiles/msfl.dir/src/xml.cpp.o: /Users/jamesweigand/Documents/msfl/src/xml.cpp
CMakeFiles/msfl.dir/src/xml.cpp.o: CMakeFiles/msfl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/jamesweigand/Documents/msfl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Building CXX object CMakeFiles/msfl.dir/src/xml.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/msfl.dir/src/xml.cpp.o -MF CMakeFiles/msfl.dir/src/xml.cpp.o.d -o CMakeFiles/msfl.dir/src/xml.cpp.o -c /Users/jamesweigand/Documents/msfl/src/xml.cpp

CMakeFiles/msfl.dir/src/xml.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/msfl.dir/src/xml.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jamesweigand/Documents/msfl/src/xml.cpp > CMakeFiles/msfl.dir/src/xml.cpp.i

CMakeFiles/msfl.dir/src/xml.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/msfl.dir/src/xml.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jamesweigand/Documents/msfl/src/xml.cpp -o CMakeFiles/msfl.dir/src/xml.cpp.s

CMakeFiles/msfl.dir/src/yaml.cpp.o: CMakeFiles/msfl.dir/flags.make
CMakeFiles/msfl.dir/src/yaml.cpp.o: /Users/jamesweigand/Documents/msfl/src/yaml.cpp
CMakeFiles/msfl.dir/src/yaml.cpp.o: CMakeFiles/msfl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/jamesweigand/Documents/msfl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_17) "Building CXX object CMakeFiles/msfl.dir/src/yaml.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/msfl.dir/src/yaml.cpp.o -MF CMakeFiles/msfl.dir/src/yaml.cpp.o.d -o CMakeFiles/msfl.dir/src/yaml.cpp.o -c /Users/jamesweigand/Documents/msfl/src/yaml.cpp

CMakeFiles/msfl.dir/src/yaml.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/msfl.dir/src/yaml.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jamesweigand/Documents/msfl/src/yaml.cpp > CMakeFiles/msfl.dir/src/yaml.cpp.i

CMakeFiles/msfl.dir/src/yaml.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/msfl.dir/src/yaml.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jamesweigand/Documents/msfl/src/yaml.cpp -o CMakeFiles/msfl.dir/src/yaml.cpp.s

CMakeFiles/msfl.dir/src/zip.cpp.o: CMakeFiles/msfl.dir/flags.make
CMakeFiles/msfl.dir/src/zip.cpp.o: /Users/jamesweigand/Documents/msfl/src/zip.cpp
CMakeFiles/msfl.dir/src/zip.cpp.o: CMakeFiles/msfl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/jamesweigand/Documents/msfl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_18) "Building CXX object CMakeFiles/msfl.dir/src/zip.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/msfl.dir/src/zip.cpp.o -MF CMakeFiles/msfl.dir/src/zip.cpp.o.d -o CMakeFiles/msfl.dir/src/zip.cpp.o -c /Users/jamesweigand/Documents/msfl/src/zip.cpp

CMakeFiles/msfl.dir/src/zip.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/msfl.dir/src/zip.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jamesweigand/Documents/msfl/src/zip.cpp > CMakeFiles/msfl.dir/src/zip.cpp.i

CMakeFiles/msfl.dir/src/zip.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/msfl.dir/src/zip.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jamesweigand/Documents/msfl/src/zip.cpp -o CMakeFiles/msfl.dir/src/zip.cpp.s

# Object files for target msfl
msfl_OBJECTS = \
"CMakeFiles/msfl.dir/src/BinUtility.cpp.o" \
"CMakeFiles/msfl.dir/src/ByteStream.cpp.o" \
"CMakeFiles/msfl.dir/src/FileWriter.cpp.o" \
"CMakeFiles/msfl.dir/src/RawBytes.cpp.o" \
"CMakeFiles/msfl.dir/src/asset.cpp.o" \
"CMakeFiles/msfl.dir/src/balloon.cpp.o" \
"CMakeFiles/msfl.dir/src/bitmap.cpp.o" \
"CMakeFiles/msfl.dir/src/bitmapRender.cpp.o" \
"CMakeFiles/msfl.dir/src/cfgParse.cpp.o" \
"CMakeFiles/msfl.dir/src/jparse.cpp.o" \
"CMakeFiles/msfl.dir/src/main.cpp.o" \
"CMakeFiles/msfl.dir/src/pak.cpp.o" \
"CMakeFiles/msfl.dir/src/png.cpp.o" \
"CMakeFiles/msfl.dir/src/ttf.cpp.o" \
"CMakeFiles/msfl.dir/src/ttfRender.cpp.o" \
"CMakeFiles/msfl.dir/src/xml.cpp.o" \
"CMakeFiles/msfl.dir/src/yaml.cpp.o" \
"CMakeFiles/msfl.dir/src/zip.cpp.o"

# External object files for target msfl
msfl_EXTERNAL_OBJECTS =

msfl: CMakeFiles/msfl.dir/src/BinUtility.cpp.o
msfl: CMakeFiles/msfl.dir/src/ByteStream.cpp.o
msfl: CMakeFiles/msfl.dir/src/FileWriter.cpp.o
msfl: CMakeFiles/msfl.dir/src/RawBytes.cpp.o
msfl: CMakeFiles/msfl.dir/src/asset.cpp.o
msfl: CMakeFiles/msfl.dir/src/balloon.cpp.o
msfl: CMakeFiles/msfl.dir/src/bitmap.cpp.o
msfl: CMakeFiles/msfl.dir/src/bitmapRender.cpp.o
msfl: CMakeFiles/msfl.dir/src/cfgParse.cpp.o
msfl: CMakeFiles/msfl.dir/src/jparse.cpp.o
msfl: CMakeFiles/msfl.dir/src/main.cpp.o
msfl: CMakeFiles/msfl.dir/src/pak.cpp.o
msfl: CMakeFiles/msfl.dir/src/png.cpp.o
msfl: CMakeFiles/msfl.dir/src/ttf.cpp.o
msfl: CMakeFiles/msfl.dir/src/ttfRender.cpp.o
msfl: CMakeFiles/msfl.dir/src/xml.cpp.o
msfl: CMakeFiles/msfl.dir/src/yaml.cpp.o
msfl: CMakeFiles/msfl.dir/src/zip.cpp.o
msfl: CMakeFiles/msfl.dir/build.make
msfl: CMakeFiles/msfl.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/jamesweigand/Documents/msfl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_19) "Linking CXX executable msfl"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/msfl.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/msfl.dir/build: msfl
.PHONY : CMakeFiles/msfl.dir/build

CMakeFiles/msfl.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/msfl.dir/cmake_clean.cmake
.PHONY : CMakeFiles/msfl.dir/clean

CMakeFiles/msfl.dir/depend:
	cd /Users/jamesweigand/Documents/msfl/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/jamesweigand/Documents/msfl /Users/jamesweigand/Documents/msfl /Users/jamesweigand/Documents/msfl/build /Users/jamesweigand/Documents/msfl/build /Users/jamesweigand/Documents/msfl/build/CMakeFiles/msfl.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/msfl.dir/depend
