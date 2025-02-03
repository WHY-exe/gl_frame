cmake_minimum_required(VERSION 3.12...3.20)
project(common)
# source code dir
aux_source_directory(${PROJECT_SOURCE_DIR}/src src)
file(GLOB include_file "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/*.h")
add_library(${PROJECT_NAME}
    ${src} 
    ${include_file}
 )
 set(PRJ_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
 target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${PRJ_INCLUDE_DIR}
    PRIVATE
    ${PRJ_INCLUDE_DIR}/${PROJECT_NAME}
 )
 
target_link_libraries(${PROJECT_NAME} 
    PUBLIC
    spdlog::spdlog
    stb_image
)

# setting up compile flags
set_property(TARGET ${PROJECT_NAME} APPEND_STRING
    PROPERTY COMPILE_FLAGS ${PROJECT_COMPILE_FLAG}
)

add_dependencies(${PROJECT_NAME} spdlog::spdlog stb_image)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${BIN_DIR}/static_lib"
        ARCHIVE_OUTPUT_DIRECTORY "${BIN_DIR}/static_lib"
        RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}/static_lib"
)