#include "first.h"

GLuint gen_buffer() {
    GLuint buffer;
    glGenBuffers(1, &buffer);
    return buffer;
}

PyList<GLuint> gen_buffers(int n) {
    PyList<GLuint> buffers(n);
    glGenBuffers(n, buffers.data_ref().data());
    return buffers;
}

GLuint gen_vertex_array() {
    GLuint array;
    glGenVertexArrays(1, &array);
    return array;
}

PyList<GLuint> gen_vertex_arrays(int n) {
    PyList<GLuint> arrays(n);
    glGenVertexArrays(n, arrays.data_ref().data());
    return arrays;
}

void shader_source(GLuint shader, PyStr &source) {
    const char *src = source.str().c_str();
    glShaderSource(shader, 1, &src, nullptr);
}

void shader_source(GLuint shader, PyList<PyStr> &sources) {
    std::vector<const char *> c_strs;
    c_strs.reserve(sources.len());
    for (int i = 0; i < sources.len(); ++i) {
        c_strs.push_back(sources[i].str().c_str());
    }
    glShaderSource(shader, static_cast<GLsizei>(c_strs.size()), c_strs.data(),
                   nullptr);
}

GLint get_shader_iv(GLuint shader, GLenum pname) {
    int param;
    glGetShaderiv(shader, pname, &param);
    return param;
}

GLint get_program_iv(GLuint program, GLenum pname) {
    int param;
    glGetProgramiv(program, pname, &param);
    return param;
}

PyStr get_shader_info_log(GLuint shader) {
    char infoLog[512];
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    return PyStr(infoLog);
}

PyStr get_program_info_log(GLuint program) {
    char infoLog[512];
    glGetProgramInfoLog(program, 512, nullptr, infoLog);
    return PyStr(infoLog);
}
