#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "stb_image.h"

struct Texture {
  unsigned int id;
  int width;
  int height;
  int bpp;
};

struct Texture *texture_create(const char* path);

void texture_destroy(struct Texture *texture);

#endif
