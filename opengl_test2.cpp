
#include <glad/gl.h>

#include "exceptions/stdexcept.h"
#include "opengl/first.h"
#include "py_list.h"
#include "py_str.h"
#include <GLFW/glfw3.h>
#include <iostream>

// Vertex shader source
pypp::PyStr _VERTEX_SHADER_SRC = pypp::PyStr(R"(
#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

out vec3 vertexColor;

void main()
{
    gl_Position = vec4(position, 1.0);
    vertexColor = color;
}
 )");

// Fragment shader source
pypp::PyList<pypp::PyStr> _FRAGMENT_SHADER_SRC = pypp::PyList({pypp::PyStr(R"(
#version 330 core
in vec3 vertexColor;
out vec4 FragColor;
)"),
                                                               pypp::PyStr(R"(
void main()
{
    FragColor = vec4(vertexColor, 1.0);
}
 )")});

GLuint compileShader(pypp::PyStr &source, GLenum type) {
    GLuint shader = glCreateShader(type);
    shader_source(shader, source);
    glCompileShader(shader);
    if (!get_shader_iv(shader, GL_COMPILE_STATUS)) {
        throw pypp::RuntimeError(pypp::PyStr("Shader compilation failed: ") +
                                 get_shader_info_log(shader));
    }
    return shader;
}

GLuint compileShader(pypp::PyList<pypp::PyStr> &sources, GLenum type) {
    GLuint shader = glCreateShader(type);
    shader_source(shader, sources);
    glCompileShader(shader);
    if (!get_shader_iv(shader, GL_COMPILE_STATUS)) {
        throw pypp::RuntimeError(pypp::PyStr("Shader compilation failed: ") +
                                 get_shader_info_log(shader));
    }
    return shader;
}

GLuint createShaderProgram() {
    GLuint vertexShader = compileShader(_VERTEX_SHADER_SRC, GL_VERTEX_SHADER);
    GLuint fragmentShader =
        compileShader(_FRAGMENT_SHADER_SRC, GL_FRAGMENT_SHADER);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    if (!get_program_iv(program, GL_LINK_STATUS)) {
        throw pypp::RuntimeError(pypp::PyStr("Program linking failed: ") +
                                 get_program_info_log(program));
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window =
        glfwCreateWindow(800, 600, "C++ OpenGL Triangle", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Load OpenGL function pointers with GLAD
    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Vertex data (positions + colors) using std::vector
    pypp::PyList<float> vertices = {
        // positions        // colors
        -0.5, -0.5, 0.0, 1.0, 0.0, 0.0, // bottom left (red)
        0.5,  -0.5, 0.0, 0.0, 1.0, 0.0, // bottom right (green)
        0.0,  0.5,  0.0, 0.0, 0.0, 1.0  // top (blue)
    };

    GLuint VAO = gen_vertex_arrays(2)[0];
    GLuint VBO = gen_buffer();

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.len() * sizeof(GLfloat),
                 vertices.data_ref().data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                          (void *)(0));
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                          (void *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Build shader program
    int shaderProgram = createShaderProgram();

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.2, 0.3, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}
