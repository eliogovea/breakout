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
#include <breakout/graphics/sprites.hpp>

namespace breakout::graphics {

class scene {
public:
  explicit scene(breakout::core::state& state);
  void render();
private:
  breakout::core::state& state_;

  shader shader_ {"../resources/shaders/generic.vs", "../resources/shaders/generic.fs"};
  
  object ball_obj_{create_object_from_file("../resources/objects/ball.obj")};
  texture ball_texture_ {"../resources/textures/ice.jpg"};
  
  object paddle_obj_{create_object_from_file("../resources/objects/cube.obj")};
  texture paddle_texture_ {"../resources/textures/wood.jpg"};
};

scene::scene(breakout::core::state& state) : state_(state) {

  // setup the view and projection matrices for every sprite only once

  glm::mat4 view = glm::lookAt(
    glm::vec3{0.0f, 0.0f, 3.0f},
    glm::vec3{0.0f, 0.0f, 0.0f},
    glm::vec3{0.0f, 1.0f, 0.0f}
  );

  glm::mat4 proj = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

  shader_.bind();
  glUniformMatrix4fv(shader_.locate_uniform("view"), 1, GL_FALSE, &view[0][0]);
  glUniformMatrix4fv(shader_.locate_uniform("proj"), 1, GL_FALSE, &proj[0][0]);
}

void scene::render() {
  // render walls, for now I will use the paddle
  {
    auto model = glm::mat4(1.0f);
    model = glm::scale(glm::vec3{0.5, 0.5, 0.5}) * model;
    model = glm::scale(glm::vec3(0.02, 2, 1)) * model;
    model = glm::translate(glm::vec3{-1.0f, 0.0f, 0.0f}) * model;
    shader_.bind();
    glUniformMatrix4fv(shader_.locate_uniform("model"), 1, GL_FALSE, &model[0][0]);  
    paddle_texture_.bind();
    paddle_obj_.render();
  }

  {
    auto model = glm::mat4(1.0f);
    model = glm::scale(glm::vec3{0.5, 0.5, 0.5}) * model;
    model = glm::scale(glm::vec3(0.02, 2, 1)) * model;
    model = glm::translate(glm::vec3{1.0, 0.0f, 0.0f}) * model;
    shader_.bind();
    glUniformMatrix4fv(shader_.locate_uniform("model"), 1, GL_FALSE, &model[0][0]);  
    paddle_texture_.bind();
    paddle_obj_.render();
  }

  {
    auto model = glm::mat4(1.0f);
    model = glm::scale(glm::vec3{0.5, 0.5, 0.5}) * model;
    model = glm::scale(glm::vec3(2, 0.02, 1)) * model;
    model = glm::translate(glm::vec3{0.0f, 1.0f, 0.0f}) * model;
    shader_.bind();
    glUniformMatrix4fv(shader_.locate_uniform("model"), 1, GL_FALSE, &model[0][0]);  
    paddle_texture_.bind();
    paddle_obj_.render();
  }

  // std::cerr << "start scene rendering ...\n";
  // std::cerr << "all balls: " << state_.get_balls().size() << "\n";
  for (auto& ball: state_.get_balls()) {
    if (!ball.is_active) {
      continue;
    }
    
    // std::cerr << "rendering a ball ...\n";
    
    auto model = glm::mat4(1.0f);
    model = glm::scale(glm::vec3(ball.radius, ball.radius, ball.radius))       * model;
    model = glm::translate(glm::vec3{ball.center.x, ball.center.y, 0.0f}) * model;
    
    shader_.bind();
    glUniformMatrix4fv(shader_.locate_uniform("model"), 1, GL_FALSE, &model[0][0]);
    
    ball_texture_.bind();
    ball_obj_.render();
  } // rendering the balls

  for (auto& paddle: state_.get_paddles()) {
    auto model = glm::mat4(1.0f);
    model = glm::scale(glm::vec3{0.5, 0.5, 0.5}) * model;
    model = glm::scale(glm::vec3(paddle.width, paddle.height, 1)) * model;
    model = glm::translate(glm::vec3{paddle.bottom_left.x + 0.5 * paddle.width, paddle.bottom_left.y + 0.5 * paddle.height, 0.0f}) * model;
    
    shader_.bind();
    glUniformMatrix4fv(shader_.locate_uniform("model"), 1, GL_FALSE, &model[0][0]);
    
    paddle_texture_.bind();
    paddle_obj_.render();
  }
}

} // namespace breakout::graphics