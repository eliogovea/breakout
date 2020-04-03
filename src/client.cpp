#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <breakout/core/state.hpp>

#include <breakout/graphics/scene.hpp>
#include <breakout/graphics/window.hpp>

using namespace breakout;

int main() {
  core::state state{};

  graphics::window window{16 * 50, 9 * 50, "breakout"};
  graphics::scene scene(state);

  double last_time = glfwGetTime();
  double curr_time;
  
  while (!window.is_closed()) {

    // TEST ONLY
    if (glfwGetKey(window.get_window(), GLFW_KEY_LEFT) == GLFW_PRESS) {
      state.get_paddles()[0].bottom_left.x -= 0.05;
      if (state.get_paddles()[0].bottom_left.x < core::state::min_x) {
        state.get_paddles()[0].bottom_left.x = core::state::min_x;
      }
    }

    if (glfwGetKey(window.get_window(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
      state.get_paddles()[0].bottom_left.x += 0.05;
      if (state.get_paddles()[0].bottom_left.x + state.get_paddles()[0].width > core::state::max_x) {
        state.get_paddles()[0].bottom_left.x = core::state::max_x - state.get_paddles()[0].width;
      }
    }

    curr_time = glfwGetTime();
    state.update(curr_time - last_time);
    last_time = curr_time;

    window.clear();
    scene.render();
    window.update();
  }
}