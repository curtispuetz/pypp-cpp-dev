#include "py_list.h"
#include "py_str.h"
#include <glad/gl.h>

GLuint gen_buffer();
PyList<GLuint> gen_buffers(int n);
GLuint gen_vertex_array();
PyList<GLuint> gen_vertex_arrays(int n);
void shader_source(GLuint shader, PyStr &source);
void shader_source(GLuint shader, PyList<PyStr> &sources);
GLint get_shader_iv(GLuint shader, GLenum pname);
GLint get_program_iv(GLuint program, GLenum pname);
PyStr get_shader_info_log(GLuint shader);
PyStr get_program_info_log(GLuint program);
