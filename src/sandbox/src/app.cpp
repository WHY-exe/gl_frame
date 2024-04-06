#include "app.h"

#include <cmath>
#include <sstream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "common/exception.h"
#include "common/util.h"
#include "gl_wrap/error.h"
#include "gl_wrap/index.h"
#include "gl_wrap/program.h"
#include "gl_wrap/shader.h"
#include "gl_wrap/vertex.h"
#include "glm/glm.hpp"
#include "spdlog/fmt/fmt.h"
#include "spdlog/spdlog.h"
namespace prj_exec1 {
bool App::InitGLFW() noexcept {
  bool ret = (glfwInit() == GLFW_TRUE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  return ret;
}

uint32_t App::InitGLEW() noexcept { return glewInit(); }

App::App() : window_() {
  if (!InitGLFW()) {
    THROW_EXCEPTION("Fail to init glfw Window", "glfw");
  }

  if (!window_.Init(640, 480, "Hello World")) {
    THROW_EXCEPTION("Fail to create glfw window", "glfw");
  }

  window_.frameBufferSizedCallback = [](int width, int height) {
    glViewport(0, 0, width, height);
  };
  window_.SetWindowCurrent();

  uint32_t glew_init_stat = InitGLEW();
  if (glew_init_stat != GLEW_OK) {
    THROW_EXCEPTION(fmt::format("Fail to init GLEW: {}",
                                (char*)glewGetErrorString(glew_init_stat)),
                    "glew");
  }
}

App::~App() { glfwTerminate(); }

void App::Run() {
  // pre-create shader and bind them to the pipeline
  gl::Program program{};
  if (program.IsInit()) {
    program.AttachShader(gl::ShaderType::VERTEX, "./shader/vertex_shader.vs");
    program.AttachShader(gl::ShaderType::FRAGMENT,
                         "./shader/fragment_shader.fs");
    program.Bind();
  } else {
    spdlog::error("fail to init glProgram");
  }
  // vertices position data
  std::vector<float> vertices = {
      0.5f,  0.5f,  0.0f, 1.0, 0.0, 0.0,  // top right
      0.5f,  -0.5f, 0.0f, 0.0, 1.0, 0.0,  // bottom right
      -0.5f, -0.5f, 0.0f, 0.0, 0.0, 1.0,  // bottom left
      //-0.5f,  0.5f, 0.0f, 1.0, 1.0, 0.0	// top left
  };
  gl::vertex::Buffer vertex_buffer{};
  vertex_buffer.SetBuffer(std::move(vertices));
  vertex_buffer.Bind();
  gl::vertex::LayoutAttri attri_pos{0};
  attri_pos.index = 0;
  attri_pos.size = 3;
  attri_pos.type = GL_FLOAT;
  attri_pos.normalize = false;
  attri_pos.stride = 6 * sizeof(float);
  attri_pos.offset = (void*)0;
  gl::vertex::LayoutAttri attri_color{0};
  attri_color.index = 1;
  attri_color.size = 3;
  attri_color.type = GL_FLOAT;
  attri_color.normalize = false;
  attri_color.stride = 6 * sizeof(float);
  attri_color.offset = (void*)(3 * sizeof(float));
  gl::vertex::Layout layout{};
  layout.Bind();
  layout.SetAttribute(attri_pos);
  layout.SetAttribute(attri_color);
  // binding index buffer
  std::vector<uint32_t> indicies = {
      0, 1, 2
      // 1, 2, 3
  };
  gl::IndexBuffer index_buffer{};
  index_buffer.SetBuffer(std::move(indicies));
  index_buffer.Bind();
  window_.renderCallback = [&program, &layout, &index_buffer]() -> bool {
    GLErrorInit;
    /* Render here */
    glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    program.Use();
    double time = glfwGetTime();
    double xOffset = ((sin(time) / 2.0f) + 0.5f) / 2.0f;
    program.SetUniformValue("xOffset", (float)xOffset);

    layout.Bind();

    // glDrawArrays(GL_TRIANGLES, 0, 6);
    GLCall(glDrawElements(GL_TRIANGLES, (int)index_buffer.GetBufferSize(),
                          GL_UNSIGNED_INT, 0));
    if (!GLErrorResult) {
      spdlog::error("encounter gl error");
    }
    return true;
  };
  window_.Start();
}

}  // namespace prj_exec1