#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

static char* load_file_as_string(const char* filename) {
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

static unsigned int compile_shader(unsigned int type, const char* source) {
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

static unsigned int create_program(const char *vertex_shader, const char* fragment_shader) {
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

int main() {
  if (!glfwInit()) return 1;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return 1;
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  unsigned int err = glewInit();
  if (err != GLEW_OK) {
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    return 1;
  }
  printf("Version: %s\n", glGetString(GL_VERSION));

  float positions[] = {
    -0.5f, -0.5f,
    0.5f, 0.5f,
    0.5f, -0.5f,
    -0.5f, 0.5f,
  };

  unsigned int indices[] = {
    0, 1, 2,
    3, 0, 1,
  };

  unsigned int vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  unsigned int buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

  unsigned int ibo;
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

  char* vertex_source = load_file_as_string("shaders/vertex_shader.glsl");
  char* fragment_source = load_file_as_string("shaders/fragment_shader.glsl");
  unsigned int program = create_program(vertex_source, fragment_source);
  free(vertex_source);
  free(fragment_source);
  if (!program) {
    glfwTerminate();
    return 1;
  }
  glUseProgram(program);
  
  int uColor = glGetUniformLocation(program, "u_Color");
  if (uColor == -1) {
      fprintf(stderr, "Failed to find uniform \"u_Color\"\n");
      return 1;
  }
  
  float r = 0.0f;
  float increment = 0.01f;
  glClearColor(1.0, 1.0, 1.0, 1.0);
  while (!glfwWindowShouldClose(window)) {
    r += increment;
    if (r >= 1.0f || r <= 0.0f) {
        increment = -increment;
    }
    glClear(GL_COLOR_BUFFER_BIT);
    glUniform4f(uColor, r, 0.3f, 0.4f, 1.0f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
}
