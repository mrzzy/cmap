#
# CMakeLists.txt
# BST Implemetation of Map
# Build file 
#

cmake_minimum_required(VERSION 3.0.0)
project(cmap)

# all: make Map Library and tests
add_library(cmap STATIC map.c)
set_target_properties(cmap PROPERTIES PUBLIC_HEADER map.h)

add_executable(test test.c)
target_link_libraries(test cmap)

# install: install Map Library to /usr/local/include
install(
    TARGETS cmap 
    ARCHIVE DESTINATION /usr/local/lib/
    PUBLIC_HEADER DESTINATION /usr/local/include/)
