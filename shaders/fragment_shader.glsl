#version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 vTexCoords;

uniform sampler2D uHeightmap;

void main() {
  float height = texture(uHeightmap, vTexCoords).r;
  vec3 color = vec3(height);
  FragColor = vec4(color, 1.0);
}
