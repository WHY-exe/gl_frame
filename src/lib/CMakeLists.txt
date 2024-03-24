cmake_minimum_required(VERSION 3.11...3.20)

file(GLOB LIBS_SUBDIRECTORIES LIST_SUBDIRECTORY true "*")

add_subdirectory(common)
add_subdirectory(gl_wrap)
add_subdirectory(glfw_wrap)