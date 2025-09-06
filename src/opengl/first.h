#include "py_list.h"
#include "py_str.h"
#include <glad/gl.h>

GLuint gen_buffer();
pypp::PyList<GLuint> gen_buffers(int n);
GLuint gen_vertex_array();
pypp::PyList<GLuint> gen_vertex_arrays(int n);
void shader_source(GLuint shader, pypp::PyStr &source);
void shader_source(GLuint shader, pypp::PyList<pypp::PyStr> &sources);
GLint get_shader_iv(GLuint shader, GLenum pname);
GLint get_program_iv(GLuint program, GLenum pname);
pypp::PyStr get_shader_info_log(GLuint shader);
pypp::PyStr get_program_info_log(GLuint program);
