#pragma once

#include <GLFW/glfw3.h>

#include <breakout/core/state.hpp>

namespace breakout::core {

class command {
public:
  virtual void execute(state& now, std::size_t id, double delta) = 0;
};

class move_left : public command {
  void execute(state& now, std::size_t id, double delta) override {
    now.paddles[id].center.x -= now.paddles[id].velocity * delta;
    if (now.paddles[id].center.x - 0.5 * now.paddles[id].width < -0.5 * now.width) {
      now.paddles[id].center.x = -0.5 * now.width + 0.5 * now.paddles[id].width;
    }
  }
};

class move_right : public command {
  void execute(state& now, std::size_t id, double delta) override {
    now.paddles[id].center.x += now.paddles[id].velocity * delta;
    if (now.paddles[id].center.x + 0.5 * now.paddles[id].width > 0.5 * now.width) {
      now.paddles[id].center.x = 0.5 * now.width - 0.5 * now.paddles[id].width;
    }
  }
};

class input_handler {
public:
  explicit input_handler();
  void handle(GLFWwindow* window, state& now, std::size_t id, double delta); // TODO
  command* left_button;
  command* right_button;
};

input_handler::input_handler() {
  left_button = new move_left();
  right_button = new move_right();
}

void input_handler::handle(GLFWwindow* window, state& now, std::size_t id, double delta) {
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    left_button->execute(now, id, delta);
  }
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    right_button->execute(now, id, delta);
  }
}

} // namespace breakout::core