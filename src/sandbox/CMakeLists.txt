# 这个项目输出可执行文件
project(sandbox)
# 让ide识别头文件
file(GLOB include_file "${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/*.h")
set(INCLUDE_BASE ${PROJECT_SOURCE_DIR}/include)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src main)
add_executable(${PROJECT_NAME} ${main} ${include_file})
# setting up include dir
target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${INCLUDE_BASE}/${PROJECT_NAME}
)

target_link_libraries(${PROJECT_NAME} 
    PUBLIC
    common
    ${OPENGL_LIBRARIES}
    glfw
    glew
    glm
    spdlog::spdlog
    gl_wrap
    glfw_wrap
)
# setting up compile flags
set_property(TARGET ${PROJECT_NAME} APPEND_STRING
    PROPERTY COMPILE_FLAGS ${PROJECT_COMPILE_FLAG}
)
add_dependencies(${PROJECT_NAME} 
    common 
    spdlog::spdlog 
    glfw 
    glew
    glm::glm    
    gl_wrap
    glfw_wrap
)