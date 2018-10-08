#
# CMakeLists.txt
# BST Implemetation of Map
# Build file 
#

cmake_minimum_required(VERSION 3.0.0)
project(cmap)

# all: make Map Library and tests
set(CMAKE_C_FLAGS "-g -Wall -Wextra -Wshadow -Wfloat-equal")
set(CMAKE_C_FLAGS_RELEASE "-Os")

add_library(cmap STATIC cmap.c)
set_target_properties(cmap PROPERTIES PUBLIC_HEADER cmap.h)

# install: install Map Library to /usr/local/include
install(
    TARGETS cmap 
    ARCHIVE DESTINATION /usr/local/lib/
    PUBLIC_HEADER DESTINATION /usr/local/include/)
