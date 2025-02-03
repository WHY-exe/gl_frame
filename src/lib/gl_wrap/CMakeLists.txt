cmake_minimum_required(VERSION 3.11...3.20)
project(gl_wrap)

set(INCLUDE_BASE "${PROJECT_SOURCE_DIR}/include")
# source code dir
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src src)
file(GLOB include_file "${INCLUDE_BASE}/lib/${PROJECT_NAME}/*.h")
add_library(${PROJECT_NAME} STATIC
    ${src} 
    ${include_file}
)
# setting up compile flags
set_property(TARGET ${PROJECT_NAME} APPEND_STRING
    PROPERTY COMPILE_FLAGS ${PROJECT_COMPILE_FLAG}
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${BIN_DIR}/static_lib"
        ARCHIVE_OUTPUT_DIRECTORY "${BIN_DIR}/static_lib"
        RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}/static_lib"
)

# setting up include dir
target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${INCLUDE_BASE}
    PRIVATE
    ${INCLUDE_BASE}/${PROJECT_NAME}
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    spdlog::spdlog
    cpptrace::cpptrace
    tl::expected
    glew_s
    common
)

add_dependencies(${PROJECT_NAME}
    common
    glew_s
    spdlog::spdlog
    cpptrace::cpptrace
)