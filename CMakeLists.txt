# cmake root file
cmake_minimum_required(VERSION 3.11...3.20)

# enter your project name here
project(gl_frame)
# specify c++ standar
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# msvc compile option
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_compile_definitions(
        _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
    )
else()
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -ggdb")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif()
# solve the cmake windows dll problem by
# specifying the global dll ouput path
# binary output dir
set(BIN_DIR "${CMAKE_BINARY_DIR}/bin")
# static lib
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BIN_DIR}/static_lib")
# shared lib
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BIN_DIR}")
# executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")

file(COPY "${CMAKE_SOURCE_DIR}/shader/" DESTINATION "${BIN_DIR}/shader")

set(THIRD_PARTY_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/third-party)

find_package(OpenGL REQUIRED)
add_subdirectory(third-party)
# cpp source dir
add_subdirectory(src)
# unit test option
option(DO_TEST "Whether compile unit test" on)
if(DO_TEST)
    enable_testing()
    add_subdirectory(test)
    add_test(NAME unit_test COMMAND unit_test)
endif()
# copy compile_commands.json to SOURCE_DIR to enable clangd magic
if (EXISTS "${CMAKE_BINARY_DIR}/compile_commands.json")
    file(COPY "${CMAKE_BINARY_DIR}/compile_commands.json" DESTINATION "${CMAKE_SOURCE_DIR}")
endif()