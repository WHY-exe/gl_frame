cmake_minimum_required(VERSION 3.11...3.20)
project(glfw_wrap)

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

# setting up include dir
target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${INCLUDE_BASE}
    PRIVATE
    ${INCLUDE_BASE}/${PROJECT_NAME}
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE
    common
    glew
    spdlog::spdlog
)
add_dependencies(${PROJECT_NAME} spdlog)