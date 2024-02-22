# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/bcr/rasp/opencv

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bcr/rasp/opencv

# Include any dependencies generated for this target.
include CMakeFiles/main.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/main.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/main.dir/flags.make

CMakeFiles/main.dir/main.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/main.cpp.o: main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bcr/rasp/opencv/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/main.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/main.cpp.o -c /home/bcr/rasp/opencv/main.cpp

CMakeFiles/main.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bcr/rasp/opencv/main.cpp > CMakeFiles/main.dir/main.cpp.i

CMakeFiles/main.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bcr/rasp/opencv/main.cpp -o CMakeFiles/main.dir/main.cpp.s

# Object files for target main
main_OBJECTS = \
"CMakeFiles/main.dir/main.cpp.o"

# External object files for target main
main_EXTERNAL_OBJECTS =

main: CMakeFiles/main.dir/main.cpp.o
main: CMakeFiles/main.dir/build.make
main: /usr/lib/arm-linux-gnueabihf/libopencv_stitching.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_alphamat.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_aruco.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_bgsegm.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_bioinspired.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_ccalib.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_dnn_objdetect.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_dnn_superres.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_dpm.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_face.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_freetype.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_fuzzy.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_hdf.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_hfs.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_img_hash.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_intensity_transform.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_line_descriptor.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_mcc.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_quality.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_rapid.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_reg.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_rgbd.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_saliency.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_shape.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_stereo.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_structured_light.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_superres.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_surface_matching.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_tracking.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_videostab.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_viz.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_xobjdetect.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_xphoto.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_highgui.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_datasets.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_plot.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_text.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_ml.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_phase_unwrapping.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_optflow.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_ximgproc.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_video.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_dnn.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_videoio.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_imgcodecs.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_objdetect.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_calib3d.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_features2d.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_flann.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_photo.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_imgproc.so.4.5.1
main: /usr/lib/arm-linux-gnueabihf/libopencv_core.so.4.5.1
main: CMakeFiles/main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bcr/rasp/opencv/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable main"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/main.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/main.dir/build: main

.PHONY : CMakeFiles/main.dir/build

CMakeFiles/main.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/main.dir/cmake_clean.cmake
.PHONY : CMakeFiles/main.dir/clean

CMakeFiles/main.dir/depend:
	cd /home/bcr/rasp/opencv && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bcr/rasp/opencv /home/bcr/rasp/opencv /home/bcr/rasp/opencv /home/bcr/rasp/opencv /home/bcr/rasp/opencv/CMakeFiles/main.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/main.dir/depend

