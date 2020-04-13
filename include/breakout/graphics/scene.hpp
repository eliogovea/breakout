#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <breakout/core/state.hpp>
#include <breakout/graphics/opengl/object.hpp>
#include <breakout/graphics/opengl/shader.hpp>
#include <breakout/graphics/opengl/texture.hpp>

#include <breakout/graphics/renderable.hpp>

namespace breakout::graphics {

class scene {
public:
  virtual void update(double delta) = 0;
  virtual scene* render() = 0;
};

class game_scene : public scene {
public:
  explicit game_scene(breakout::core::state* state);
  void update(double delta) override;
  scene* render() override;

  void set_state(breakout::core::state* state);
private:
  renderable ball_ = create_ball();
  renderable paddle_ = create_paddle();
  renderable brick_ = create_brick();
  breakout::core::state* state_;
};

game_scene::game_scene(breakout::core::state* state) : state_(state) {}

void game_scene::set_state(breakout::core::state* state) {
  state_ = state;
}

void game_scene::update(double delta) {
  state_->update(delta);
}

scene* game_scene::render() {
  static glm::mat4 view = glm::lookAt(
    glm::vec3{0.0f, 0.0f, 3.0f},
    glm::vec3{0.0f, 0.0f, 0.0f},
    glm::vec3{0.0f, 1.0f, 0.0f}
  );

  static glm::mat4 proj = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

  { // rendering the balls
    for (auto& ball: state_->balls) {
      if (ball.strength == 0) {
        continue;
      }
      auto model = glm::mat4(1.0f);
      model = glm::scale(glm::vec3(ball.radius, ball.radius, ball.radius))  * model;
      model = glm::translate(glm::vec3{ball.center.x, ball.center.y, 0.0f}) * model;

      ball_.render(model, view, proj);
    }
  }

  { // rendering thte paddles
    for (auto& paddle: state_->paddles) {
      auto model = glm::mat4(1.0f);
      model = glm::scale(glm::vec3{0.5, 0.5, 0.5}) * model;
      model = glm::scale(glm::vec3(paddle.width, paddle.height, 1)) * model;
      model = glm::translate(glm::vec3{paddle.center.x, paddle.center.y, 0.0f}) * model;
      paddle_.render(model, view, proj);
    }
  }

  { // rendering thte bricks
    for (auto& brick: state_->bricks) {
      if (brick.strength == 0) {
        continue;
      }
      auto model = glm::mat4(1.0f);
      model = glm::scale(glm::vec3{0.5, 0.5, 0.5}) * model;
      model = glm::scale(glm::vec3(brick.width, brick.height, 1)) * model;
      model = glm::translate(glm::vec3{brick.center.x, brick.center.y, 0.0f}) * model;
      brick_.render(model, view, proj);
    }
  }
  return this;
}

} // namespace breakout::graphics