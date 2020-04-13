#pragma once

#include <breakout/graphics/opengl/object.hpp>
#include <breakout/graphics/opengl/shader.hpp>
#include <breakout/graphics/opengl/texture.hpp>

namespace breakout::graphics {

class renderable {
public:
  explicit renderable(object* object_, shader* shader_, texture* texture_);
  ~renderable() = default; // TODO
  void render(const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj);
  object* object_;
  shader* shader_;
  texture* texture_;
};

renderable::renderable(object* object_, shader* shader_, texture* texture_)
  : object_(object_), shader_(shader_), texture_(texture_) {}

void renderable::render(const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj) {
  shader_->bind();
  glUniformMatrix4fv(shader_->locate_uniform("model"), 1, GL_FALSE, &model[0][0]);
  glUniformMatrix4fv(shader_->locate_uniform("view"), 1, GL_FALSE, &view[0][0]);
  glUniformMatrix4fv(shader_->locate_uniform("proj"), 1, GL_FALSE, &proj[0][0]);
  texture_->bind();
  object_->render();
}

inline static renderable create_ball() {
  return renderable{
    create_object_from_file("../resources/objects/ball.obj"),
    new shader("../resources/shaders/generic.vs", "../resources/shaders/generic.fs"),
    new texture("../resources/textures/dots.jpeg"),
  };
}

inline static renderable create_paddle() {
  return renderable{
    create_object_from_file("../resources/objects/cube.obj"),
    new shader("../resources/shaders/generic.vs", "../resources/shaders/generic.fs"),
    new texture("../resources/textures/metal.jpg"),
  };
}

inline static renderable create_brick() {
  return renderable{
    create_object_from_file("../resources/objects/cube.obj"),
    new shader("../resources/shaders/generic.vs", "../resources/shaders/generic.fs"),
    new texture("../resources/textures/ice.jpg"),
  };
}

} // namespace breakout::graphics