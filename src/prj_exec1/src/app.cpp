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

App::App() : m_window() {
  if (!InitGLFW()) {
    THROW_BASIC_EXCEPTION("Fail to init glfw Window");
  }

  if (!m_window.Init(640, 480, "Hello World")) {
    THROW_BASIC_EXCEPTION("Fail to create glfw window");
  }

  m_window.OnFrameBufferSized([](glfw::Window& window, int width, int height) {
    glViewport(0, 0, width, height);
  });
  m_window.SetWindowCurrent();

  uint32_t glew_init_stat = InitGLEW();
  if (glew_init_stat != GLEW_OK) {
    std::stringstream ss;
    ss << "Fail to init GLEW: " << (char*)glewGetErrorString(glew_init_stat);
    THROW_BASIC_EXCEPTION(ss.str());
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
    program.Link();
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
  vertex_buffer.Bind(vertices);
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
  gl::index::Buffer index_buffer{};
  index_buffer.Bind(indicies);
  m_window.Update([&program, &layout, &indicies](glfw::Window& window) -> bool {
    /* Render here */
    glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    program.Install();
    float time = glfwGetTime();
    float xOffset = ((sin(time) / 2.0f) + 0.5f) / 2.0f;
    program.SetUniformValue("xOffset", xOffset);

    layout.Bind();

    // glDrawArrays(GL_TRIANGLES, 0, 6);
    GLCall(glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, 0));
    return true;
  });
  m_window.Run();
}

}  // namespace prj_exec1