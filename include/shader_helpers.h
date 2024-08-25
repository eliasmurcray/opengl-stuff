#ifndef SHADER_HELPERS_H
#define SHADER_HELPERS_H

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>

char* load_file_as_string(const char* filename);

unsigned int compile_shader(unsigned int type, const char* source);

unsigned int create_program(const char *vertex_shader, const char* fragment_shader);

#endif
