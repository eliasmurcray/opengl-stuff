#include "texture.h"

struct Texture *texture_create(const char* path) {
  struct Texture *texture = malloc(sizeof(struct Texture));
  if (!texture) return NULL;
  texture->id = 0;
  texture->width = 0;
  texture->height = 0;
  texture->bpp = 0;
  stbi_set_flip_vertically_on_load(1);
  unsigned char *data = stbi_load(path, &texture->width, &texture->height, &texture->bpp, 4);
  if (!data) {
    free(texture);
    fprintf(stderr, "Failed to load texture from %s\n%s\n", path, stbi_failure_reason());
    return NULL;
  }
  glGenTextures(1, &texture->id);
  glBindTexture(GL_TEXTURE_2D, texture->id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glBindTexture(GL_TEXTURE_2D, 0);
  stbi_image_free(data);
  return texture;
}

void texture_destroy(struct Texture *texture) {
    glDeleteTextures(1, &texture->id);
    free(texture);
}
