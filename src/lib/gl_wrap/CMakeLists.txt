cmake_minimum_required(VERSION 3.11...3.20)
project(gl_wrap)
# source code dir
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src src)
file(GLOB include_file "${INCLUDE_BASE}/lib/${PROJECT_NAME}/*.h")
add_library(${PROJECT_NAME} STATIC
    ${src} 
    ${include_file}
)

# setting up include dir
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${INCLUDE_BASE}/lib/${PROJECT_NAME}
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE
    spdlog::spdlog
)

add_dependencies(${PROJECT_NAME} spdlog)