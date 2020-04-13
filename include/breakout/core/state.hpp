#pragma once

#include <iostream>

#include <vector>

#include <glm/glm.hpp>

#include "util.hpp"

namespace breakout::core {

struct ball {
  glm::vec2 center;
  glm::float32 radius;
  glm::vec2 velocity;
  int32_t strength;
};

struct brick {
  glm::vec2 center;
  glm::float32 width;
  glm::float32 height;
  int32_t strength;
};

struct paddle {
  glm::vec2 center;
  glm::float32 velocity;
  glm::float32 width;
  glm::float32 height;
  int32_t strength;
};

struct state {
  void update(glm::float32 delta);
  glm::float32 width;
  glm::float32 height;
  std::vector<paddle> paddles;
  std::vector<ball> balls;
  std::vector<brick> bricks;
};

void state::update(glm::float32 delta) {
  for (auto& ball: balls) {
    if (ball.strength == 0) {
      continue;
    }
    ball.center += ball.velocity * delta;
    
    // check boundaries
    if (ball.center.y - ball.radius >= 0.5 * height) {
      ball.velocity.y = -ball.velocity.y;
    }
    if (ball.center.x - ball.radius <= -0.5 * width) {
      ball.velocity.x = -ball.velocity.x;
    }
    if (ball.center.x - ball.radius >= 0.5 * width) {
      ball.velocity.x = -ball.velocity.x;
    }
 
    for (auto& paddle: paddles) {
      if (paddle.strength == 0) {
        continue;
      }
      if (util::collide({ball.center, ball.radius}, {paddle.center, paddle.width, paddle.height})) {
        ball.velocity.y = -ball.velocity.y;
      }
    }

    for (auto& ball: balls) {
      if (ball.center.y + ball.radius < -height) {
        ball.strength = 0;
      }
    }

    for (auto& ball: balls) {
      if (ball.strength == 0) {
        continue;
      }
      for (auto& brick: bricks) {
        if (brick.strength == 0) {
          continue;
        }
        if (util::collide({ball.center, ball.radius}, {brick.center, brick.width, brick.height})) {
          brick.strength--;
          ball.velocity.y = -ball.velocity.y;
        }
      }
    }
  }
}

}