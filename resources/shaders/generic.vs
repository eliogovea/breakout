#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texture_uv;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec3 fragment_color;
out vec2 fragment_texture_uv;

void main() {
	gl_Position = proj * view * model * vec4(position, 1);
  fragment_color = color;
  fragment_texture_uv = texture_uv;
}
