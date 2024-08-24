#include "app.h"
#include "common/exception.h"
#include "common/image_loader.h"
#include "glfw_wrap/context.h"
#include "gl_wrap/error.h"
#include "gl_wrap/index.h"
#include "gl_wrap/program.h"
#include "gl_wrap/shader.h"
#include "gl_wrap/texture.h"
#include "gl_wrap/vertex.h"
#include "spdlog/spdlog.h"
#include <cmath>

namespace sandbox {
App::App() : context_(3, 3) {
    if (!InitWindow()) {
        THROW_EXCEPTION("Fail to init glfw window", "glfw");
    }
    uint32_t glew_init_stat;
    if ((glew_init_stat = glewInit()) != GLEW_OK) {
        const auto error_msg =
            fmt::format("fail to init glew: {}", (char *)glewGetErrorString(glew_init_stat));
        THROW_EXCEPTION(error_msg.c_str(), "glew");
    }
}

bool App::InitWindow() noexcept {
    bool ret                         = window_.Init(640, 480, "Hello World");
    window_.frameBufferSizedCallback = [](int width, int height) {
        glViewport(0, 0, width, height);
    };
    window_.SetWindowCurrent();
    return ret;
}

void App::DoLogic() noexcept {
    if (window_.kbd.KeyIsPressed(GLFW_KEY_A)) {
        SPDLOG_INFO("a key is pressed");
    }
}

void App::DoRender(gl::Program &program) noexcept {
    GLErrorInit;
    /* Render here */
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    program.Use();
    double time     = glfwGetTime();
    double x_offset = ((sin(time) / 2.0f) + 0.5f) / 2.0f;
    program.SetUniformValue("xOffset", static_cast<float>(x_offset));

    // glDrawArrays(GL_TRIANGLES, 0, 6);
    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
    if (!GLErrorResult) {
        SPDLOG_ERROR("encounter gl error");
    }
}

int App::Run() {
    // pre-create shader and bind them to the pipeline
    gl::Program program{};
    if (program.IsInit()) {
        program.AttachShader(gl::ShaderType::VERTEX, "./shader/vertex_shader.vs");
        program.AttachShader(gl::ShaderType::FRAGMENT, "./shader/fragment_shader.fs");
        program.Bind();
    } else {
        SPDLOG_ERROR("fail to init glProgram");
    }
    // vertices position data
    std::vector<float> vertices = {
        0.5f,  0.5f,  0.0f, 1.0, 0.0, 0.0, 1.0f, 1.0f, // top right
        0.5f,  -0.5f, 0.0f, 0.0, 1.0, 0.0, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0, 0.0, 1.0, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f,  0.0f, 1.0, 1.0, 0.0, 0.0f, 1.0f, // top left
    };
    gl::vertex::Buffer vertex_buffer{};
    vertex_buffer.SetBuffer(std::move(vertices));
    vertex_buffer.Bind();
    gl::Texture2D tex{};
    tex.Bind();
    tex.SetParam({
        {GL_TEXTURE_WRAP_S, GL_REPEAT},
        {GL_TEXTURE_WRAP_T, GL_REPEAT},
        {GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR},
        {GL_TEXTURE_MAG_FILTER, GL_LINEAR},
    });
    tex.SetData(util::ImageLoader("assets/wall.jpg"));
    gl::vertex::LayoutAttri attri_pos{};
    attri_pos.index     = 0;
    attri_pos.size      = 3;
    attri_pos.type      = GL_FLOAT;
    attri_pos.normalize = false;
    attri_pos.stride    = 8 * sizeof(float);
    attri_pos.offset    = (void *)nullptr;
    gl::vertex::LayoutAttri attri_color{};
    attri_color.index     = 1;
    attri_color.size      = 3;
    attri_color.type      = GL_FLOAT;
    attri_color.normalize = false;
    attri_color.stride    = 8 * sizeof(float);
    attri_color.offset    = (void *)(3 * sizeof(float));
    gl::vertex::LayoutAttri attri_uv{};
    attri_uv.index     = 2;
    attri_uv.size      = 2;
    attri_uv.type      = GL_FLOAT;
    attri_uv.normalize = false;
    attri_uv.stride    = 8 * sizeof(float);
    attri_uv.offset    = (void *)(6 * sizeof(float));
    gl::vertex::Layout layout{};
    layout.Bind();
    layout.SetAttribute(attri_pos);
    layout.SetAttribute(attri_color);
    layout.SetAttribute(attri_uv);
    // binding index buffer
    std::vector<uint32_t> indicies = {0, 1, 2, 3, 2, 0};
    gl::IndexBuffer       index_buffer{};
    index_buffer.SetBuffer(std::move(indicies));
    index_buffer.Bind();
    while (!window_.ShouldClose()) {
        DoLogic();
        GLErrorInit;
        /* Render here */
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        program.Use();
        double time     = glfwGetTime();
        double x_offset = ((sin(time) / 2.0f) + 0.5f) / 2.0f;
        program.SetUniformValue("xOffset", static_cast<float>(x_offset));

        tex.Bind();
        layout.Bind();
        // glDrawArrays(GL_TRIANGLES, 0, 6);
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
        if (!GLErrorResult) {
            SPDLOG_ERROR("encounter gl error");
        }
        /* Swap front and back buffers */
        window_.SwapBuffer();
        /* Poll for and process events */
        glfw::PollEvents();
    }
    return 0;
}

} // namespace sandbox