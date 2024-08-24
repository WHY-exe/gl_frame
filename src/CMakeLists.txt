cmake_minimum_required(VERSION 3.11...3.20)

set(PROJECT_COMPILE_FLAG )
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_definitions(-DNOMINMAX)
    set(PROJECT_COMPILE_FLAG "${CMAKE_CXX_FLAGS} /W4 /WX")
else()
    set(PROJECT_COMPILE_FLAG "${CMAKE_CXX_FLAGS} -Wall -Werror")
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND UNIX)
        set(PROJECT_COMPILE_FLAG "${PROJECT_COMPILE_FLAG} -lpthread")
    endif()
endif()

add_compile_definitions(
    SPDLOG_COMPILED_LIB
)

include_directories(
    ${THIRD_PARTY_INCLUDE}/glew-2.2.0/include
    ${THIRD_PARTY_INCLUDE}/glfw/include
    ${THIRD_PARTY_INCLUDE}/glm
)
add_subdirectory(lib)
add_subdirectory(sandbox)