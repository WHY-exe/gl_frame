# cmake root file
cmake_minimum_required(VERSION 3.11...3.20)

# enter your project name here
project(gl_frame)
# specify c++ standar
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# msvc compile option
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # set debug compile option
    if (POLICY CMP0141)
        cmake_policy(SET CMP0141 NEW)
        set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF: $< AND: $<C_COMPILER_ID:MSVC>, $<CXX_COMPILER_ID:MSVC> >, $< $<CONFIG:Debug,RelWithDebInfo>:EditAndContinue >, $< $<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
    endif()
    # active multi-thread compliation flag on msvc
    cmake_host_system_information(RESULT CPU_NUMBER_OF_LOGICAL_CORES QUERY NUMBER_OF_LOGICAL_CORES)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP${CPU_NUMBER_OF_LOGICAL_CORES}")
    add_compile_definitions(
        _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
    )
else()
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -ggdb")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif()
# solve the cmake windows dll problem by
# specifying the global dll ouput path
set(BIN_DIR "${CMAKE_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE}")
# static lib
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BIN_DIR}/static_lib")
# shared lib
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BIN_DIR}")
# executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")

file(COPY "${CMAKE_SOURCE_DIR}/shader/" DESTINATION "${BIN_OUT_DIR}/shader")

# public include dir
set(INCLUDE_BASE ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(THIRD_PARTY_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/third-party)

find_package(OpenGL REQUIRED)
# cpp source dir
add_subdirectory(src)
# turn this option off if you don't wnat to rebuild third-party lib
option(BUILD_THIRD_PARTY_LIB "Building third-party dependencies" on)
if (BUILD_THIRD_PARTY_LIB)
    add_subdirectory(third-party)
endif()
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