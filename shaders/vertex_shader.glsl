#version 330 core

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aTexCoords;

out vec2 vTexCoords;
out vec2 pix;

void main() {
  gl_Position = vec4(aPosition.xy, 0.0, 1.0);
  vTexCoords = aTexCoords;
  pix = aPosition.xy;
}
