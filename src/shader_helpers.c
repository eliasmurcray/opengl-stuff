#include "shader_helpers.h"

char* load_file_as_string(const char* filename) {
  FILE* file = fopen(filename, "r");
  if (!file) return NULL;
  fseek(file, 0, SEEK_END);
  long len = ftell(file);
  fseek(file, 0, SEEK_SET);
  char *buffer = (char *)malloc(len + 1);
  if (buffer) {
    fread(buffer, 1, len, file);
    buffer[len] = '\0';
  }
  fclose(file);
  return buffer;
}

unsigned int compile_shader(unsigned int type, const char* source) {
  if (!source) {
    fprintf(stderr, "ERROR::SHADER::NULL_SOURCE\n");
    return 0;
  }
  unsigned int shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);
  int compiled;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
  if (compiled == GL_TRUE) return shader;
  int log_length;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
  char message[log_length];
  glGetShaderInfoLog(shader, log_length - 1, NULL, message);
  fprintf(stderr, "ERROR::SHADER::COMPILATION_FAILED\n%s\n", message);
  glDeleteShader(shader);
  return 0;
}

unsigned int create_program(const char *vertex_shader, const char* fragment_shader) {
  unsigned int program = glCreateProgram();
  unsigned int vshader = compile_shader(GL_VERTEX_SHADER, vertex_shader);
  if (!vshader) return 0;
  unsigned int fshader = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);
  if (!fshader) return 0;
  glAttachShader(program, vshader);
  glAttachShader(program, fshader);
  glLinkProgram(program);
  glDeleteShader(vshader);
  glDeleteShader(fshader);
  int program_linked;
  glGetProgramiv(program, GL_LINK_STATUS, &program_linked);
  if (program_linked == GL_TRUE) return program;
  int log_length;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
  char message[log_length];
  glGetProgramInfoLog(program, log_length - 1, NULL, message);
  fprintf(stderr, "ERROR::PROGRAM::LINKING_FAILED\n%s\n", message);
  glDeleteProgram(program);
  return 0;
}
