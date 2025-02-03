cmake_minimum_required(VERSION 3.11...3.20)

find_package(Threads REQUIRED)
set(PROJECT_COMPILE_FLAG )
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_definitions(-DNOMINMAX)
    set(PROJECT_COMPILE_FLAG "${CMAKE_CXX_FLAGS} /W4 ")
else()
    set(PROJECT_COMPILE_FLAG "${CMAKE_CXX_FLAGS} -Wall -Werror")
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND UNIX)
        set(PROJECT_COMPILE_FLAG "${PROJECT_COMPILE_FLAG} -lpthread")
    endif()
endif()

include_directories(
    ${THIRD_PARTY_INCLUDE}/glew-2.2.0/include
    ${THIRD_PARTY_INCLUDE}/glfw/include
    ${THIRD_PARTY_INCLUDE}/glm
)

cmrc_add_resource_library(
    glsl-shader
    ALIAS glsl::shader
    NAMESPACE glsl
    shader/fragment_shader.fs
    shader/vertex_shader.vs
)

add_subdirectory(lib)
add_subdirectory(sandbox)