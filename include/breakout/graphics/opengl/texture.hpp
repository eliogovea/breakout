#pragma once

#include <iostream>
#include <string>

#include <GL/glew.h>

#include <vendor/stb_image.h>

namespace breakout::graphics {

class texture {
public:
  explicit texture(const std::string& file_path);
  ~texture();
  void bind();
private:
  GLuint texture_id;
};

texture::texture(const std::string& file_path) {
  std::cout << "loading texture from " << file_path << std::endl;
  
  int width, height, components;
  unsigned char* data = stbi_load(file_path.c_str(), &width, &height, &components, 4);

  if (data == nullptr) {
    std::cout << "error loading texture from " << file_path << std::endl;
  }

  glGenTextures(1, &texture_id);

  std::cout << "texture id: " << texture_id << std::endl;

  glBindTexture(GL_TEXTURE_2D, texture_id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

  stbi_image_free(data);
}

texture::~texture() {
  glDeleteTextures(1, &texture_id);
}

void texture::bind() {
  glBindTexture(GL_TEXTURE_2D, texture_id);
}


} // namespace breakout::graphics