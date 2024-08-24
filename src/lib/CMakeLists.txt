cmake_minimum_required(VERSION 3.11...3.20)
if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()
set(POSITION_INDEPENDENT_CODE on)
# add subdirectory for other third party lib
add_subdirectory(common)
add_subdirectory(gl_wrap)
add_subdirectory(glfw_wrap)