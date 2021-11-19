# Refer to the dockerized SDK thread on mobileread.com.
cmake_minimum_required (VERSION 3.21)

set (CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set (EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set (LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

include ("${CMAKE_TOOLCHAIN_FILE}")

string (REGEX REPLACE "[^/]+$" "pbres" PBRES "${CMAKE_C_COMPILER}")

link_directories ("${PB_LINK_DIRECTORIES}")

include_directories ("${PB_INCLUDE_DIRECTORIES}")
include_directories ("${FREETYPE_INCLUDE_DIRS}")

#
# After the essential setup, the actual CMakeLists.
#

project (Notepad VERSION 0.2)
# I'm sure there's a more elegant way to do the following.
set (CMAKE_EXE_LINKER_FLAGS "-linkview -lm")

add_executable (Notepad.app main.c src/event.c src/ui.c)
