#include "opengl/first.h"
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <iostream>

#include <vector>

// Vertex shader source
#include <string>
std::string VERTEX_SHADER_SRC = R"(
#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

out vec3 vertexColor;

void main()
{
    gl_Position = vec4(position, 1.0);
    vertexColor = color;
}
 )";

// Fragment shader source
std::string FRAGMENT_SHADER_SRC = R"(
#version 330 core
in vec3 vertexColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(vertexColor, 1.0);
}
 )";

int lib_gl_get_shader_iv(GLuint shader, GLenum pname) {
    int param;
    glGetShaderiv(shader, pname, &param);
    return param;
}

int lib_gl_get_program_iv(GLuint program, GLenum pname) {
    int param;
    glGetProgramiv(program, pname, &param);
    return param;
}

std::string lib_gl_get_shader_info_log(GLuint shader) {
    char infoLog[512];
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    return infoLog;
}

std::string lib_gl_get_program_info_log(GLuint program) {
    char infoLog[512];
    glGetProgramInfoLog(program, 512, nullptr, infoLog);
    return infoLog;
}

void lib_gl_shader_source(GLuint shader, std::string &source) {
    // Don't support the other two arguments for this one.
    const char *src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
}

int compileShader(std::string &source, GLenum type) {
    int shader = glCreateShader(type);
    lib_gl_shader_source(shader, source);
    glCompileShader(shader);
    if (!lib_gl_get_shader_iv(shader, GL_COMPILE_STATUS)) {
        throw std::runtime_error("Shader compilation failed: " +
                                 lib_gl_get_shader_info_log(shader));
    }
    return shader;
}

int createShaderProgram() {
    int vertexShader = compileShader(VERTEX_SHADER_SRC, GL_VERTEX_SHADER);
    int fragmentShader = compileShader(FRAGMENT_SHADER_SRC, GL_FRAGMENT_SHADER);

    int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    if (!lib_gl_get_program_iv(program, GL_LINK_STATUS)) {
        throw std::runtime_error("Program linking failed: " +
                                 lib_gl_get_program_info_log(program));
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
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Vertex data (positions + colors) using std::vector
    std::vector<float> vertices = {
        // positions        // colors
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left (red)
        0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right (green)
        0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f  // top (blue)
    };

    GLuint VAO = gen_vertex_array();
    GLuint VBO = gen_buffer();

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
                 vertices.data(), GL_STATIC_DRAW);

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
