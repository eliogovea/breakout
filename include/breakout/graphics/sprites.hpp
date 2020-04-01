#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <breakout/graphics/opengl/object.hpp>
#include <breakout/graphics/opengl/shader.hpp>
#include <breakout/graphics/opengl/texture.hpp>

namespace breakout::graphics {

namespace sprites {

class sprite {
public:
  explicit sprite(object& obj_, shader& shader_, texture& texture_);
  void render();
  shader& get_shader();
private:
  object& obj_;
  shader& shader_;
  texture& texture_;
  
};

sprite::sprite(object& obj_, shader& shader_, texture& texture_) 
: obj_(obj_), shader_(shader_), texture_(texture_) {}

void sprite::render() {
  shader_.bind();
  obj_.render();
}

shader& sprite::get_shader() {
  return shader_;
}


// TODO helper functions
// create_ball_sprite
// create_box_sprite
// create_brick_sprite
// create_paddle_sprite

} // namespace sprites

} // namespace breakout::client::graphics