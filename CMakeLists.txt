# cmake root file
cmake_minimum_required(VERSION 3.11...3.20)

# enter your project name here
project(gl_frame)
# C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# msvc 编译选项
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # 设置Debug编译选项
    if (POLICY CMP0141)
        cmake_policy(SET CMP0141 NEW)
        set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT 
        "$<IF: $< AND: $<C_COMPILER_ID:MSVC>, $<CXX_COMPILER_ID:MSVC> >, $< $<CONFIG:Debug,RelWithDebInfo>:EditAndContinue >, $< $<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase >>")
    endif()
    # 开启多线程编译
    cmake_host_system_information(RESULT CPU_NUMBER_OF_LOGICAL_CORES QUERY NUMBER_OF_LOGICAL_CORES)
    # 设置Release编译选项
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /MP${CPU_NUMBER_OF_LOGICAL_CORES}")
# gcc编译选项
else()
     # 设置Debug编译选项
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -ggdb")
    # 设置Release编译选项
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread")
    endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    add_definitions(-DLINUX)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    add_definitions(-DWINDOWS)
endif()

add_definitions(-DSPDLOG_COMPILED_LIB)
add_definitions(-D_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING)
# solve the cmake windows dll problem by
# specifying the global dll ouput path
set(BIN_OUT_DIR "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BIN_OUT_DIR}/static_lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BIN_OUT_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BIN_OUT_DIR}")

file(COPY "${CMAKE_SOURCE_DIR}/shader/" DESTINATION "${BIN_OUT_DIR}/shader")
# add library linking dir
link_directories(
    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
)
# public include dir
set(INCLUDE_BASE ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(THIRD_PARTY_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/third-party)
include_directories(
    ${INCLUDE_BASE}
    ${INCLUDE_BASE}/lib
    ${THIRD_PARTY_INCLUDE}/assimp/include
    ${THIRD_PARTY_INCLUDE}/glew-cmake/include
    ${THIRD_PARTY_INCLUDE}/glfw/include
    ${THIRD_PARTY_INCLUDE}/glm
    ${THIRD_PARTY_INCLUDE}/spdlog/include
)
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