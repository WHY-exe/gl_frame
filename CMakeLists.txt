# cmake root file
cmake_minimum_required(VERSION 3.11...3.20)

# enter your project name here
project(gl_frame LANGUAGES C CXX)
option(BUILD_EXAMPLE OFF)
option(BUILD_TEST "Whether compile unit test" OFF)
# C++ standar
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# msvc compile option
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # set debug compile option
    if (POLICY CMP0141)
        cmake_policy(SET CMP0141 NEW)
        set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF: $< AND: $<C_COMPILER_ID:MSVC>, $<CXX_COMPILER_ID:MSVC> >, $< $<CONFIG:Debug,RelWithDebInfo>:EditAndContinue >, $< $<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
    endif()
    add_compile_definitions(
        _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
    )
endif()

# public include dir
set(THIRD_PARTY_INCLUDE "${PROJECT_SOURCE_DIR}/third-party")
# binary output dir
set(BIN_DIR "${CMAKE_BINARY_DIR}/../bin/${CMAKE_BUILD_TYPE}/")
# example binary output dir
set(EXAMPLE_DIR "${CMAKE_BINARY_DIR}/example")
# test binary output dir
set(TEST_DIR "${CMAKE_BINARY_DIR}/test")

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/CMakeRC/CMakeRC.cmake")

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