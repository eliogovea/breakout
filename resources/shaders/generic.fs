#version 330 core

in vec3 fragment_color;
in vec2 fragment_texture_uv;

out vec4 color;

uniform sampler2D texture_sampler;

void main() {
	// color = fragment_color;
  color = texture(texture_sampler, fragment_texture_uv);
}
