#pragma once

#include <iostream>

#include <vector>

#include <glm/glm.hpp>

#include "util.hpp"

namespace breakout::core {

enum class phase {
  waiting,
  playing,
  win,
  loss
};

struct paddle : public util::rectangle {
  int32_t strength{1};
};

struct ball : public util::circle {
  glm::vec2 velocity{1};
  bool is_active{true};
  int32_t strength{1};

  void update(float dt) {
    center += velocity * dt;
  }
};

struct block : public util::rectangle {
  bool is_active{true};
  int32_t strength{1};
};

class state {
public:
  explicit state();
  void update(float dt);
  std::vector<ball>& get_balls();
  std::vector<block>& get_blocks();
  std::vector<paddle>& get_paddles();
  static constexpr float min_x{-1.0f};
  static constexpr float max_x{+1.0f};
  static constexpr float min_y{-1.0f};
  static constexpr float max_y{+1.0f};
private:
  int32_t active_balls_count_{0};
  int32_t active_blocks_count_{0};
  std::vector<paddle> paddles_;
  std::vector<ball> balls_;
  std::vector<block> blocks_;
  phase phase_{phase::playing}; // TODO change to waiting

};

state::state() {
  // TEST only !!!
  ball b; 
  b.center = glm::vec2{0.0f, 0.0f};
  b.radius = 0.1;
  b.strength = 1;
  b.is_active = true;
  b.velocity = {2.0f, 2.0f};
  balls_.push_back(b);
  active_balls_count_ = 1;

  paddle p;
  p.bottom_left = {0.0f, -1.0f};
  p.width = 0.5;
  p.height = 0.05;

  paddles_.push_back(p);
}

void state::update(float dt) {
  if (phase_ != phase::playing) {
    return;
  }
  std::cout << "UPDATE!!!\n";
  for (auto& ball: balls_) {
    ball.update(dt);
    if (ball.center.x - ball.radius <= min_x) {
      ball.center.x = min_x + ball.radius;
      ball.velocity.x = -ball.velocity.x;
    }
    if (ball.center.x + ball.radius > max_x) {
      ball.center.x = max_x - ball.radius;
      ball.velocity.x = -ball.velocity.x;
    }
    if (ball.center.y + ball.radius > max_y) {
      ball.center.y = max_y - ball.radius;
      ball.velocity.y = -ball.velocity.y;
    }
  }
  std::cout << "CHECK COLLISION\n";
  for (auto& ball: balls_) {
    if (!ball.is_active) {
      continue;
    }
    for (auto& block: blocks_) {
      if (block.is_active && util::collides(ball, block)) {
        block.strength -= ball.strength;
        if (block.strength < 0) {
          block.is_active = false;
        }
        if (ball.center.y < block.bottom_left.y) {
          ball.velocity.y = -ball.velocity.y;
        }
        if (ball.center.x < block.bottom_left.x) {
          ball.velocity.x = ball.velocity.x;
        }
      }
    }
  }
  for (auto& ball: balls_) {
    bool collide = false;
    for (auto& paddle: paddles_) {
      if (ball.center.y - ball.radius <= paddle.bottom_left.y + paddle.height) {
        if (paddle.bottom_left.x <= ball.center.x && ball.center.x <= paddle.bottom_left.x + paddle.width) {
          collide = true;
          break;
        }
      }
    }
    if (collide && ball.velocity.y < 0) {
      std::cout << "COLLIDE!!!\n";
      ball.velocity.y = -ball.velocity.y;
    }
  }
  for (auto& ball: balls_) {
    if (!ball.is_active) {
      continue;
    }
    if (ball.center.y <= min_y) {
      ball.is_active = false;
      active_balls_count_--;
    }
  }
  if (active_balls_count_ == 0) {
    std::cout << "LOSS!!!\n";
    phase_ = phase::loss;
    return;
  }
  if (active_blocks_count_ == 0) {
    // std::cout << "WIN!!!\n";
    // phase_ = phase::win;
    return;
  }
  for (auto& ball: balls_) {
    if (ball.center.y <= min_y) {
      ball.is_active = false;
    }
  }
  
}

std::vector<ball>& state::get_balls() {
  return balls_;
}

std::vector<block>& state::get_blocks() {
  return blocks_;
}

std::vector<paddle>& state::get_paddles() {
  return paddles_;
}

}