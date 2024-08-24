#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
/*#include <stdlib.h>*/
#include "shader_helpers.h"

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
