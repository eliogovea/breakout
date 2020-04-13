#include <thread>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <breakout/core/state.hpp>
#include <breakout/core/state_reader.hpp>
#include <breakout/core/input_handler.hpp>

#include <breakout/graphics/scene.hpp>
#include <breakout/graphics/window.hpp>

#include <breakout/network/client.hpp>

using namespace breakout;

int main() {
  graphics::window window{16 * 50, 9 * 50, "breakout"};
  network::handler client{"127.0.0.1", 1515, 9000};

  std::cout << "sending join request\n";
  client.request_join();
  // TODO sleep

  std::cout << "listening\n";
  std::thread network_thread([&] {
    client.listen();
  });

  int64_t last_state_id = client.get_state_id();
  breakout::core::state state = client.get_last_state();

  core::input_handler input{};
  
  graphics::scene* scene = new graphics::game_scene(&state);

  double last_time = glfwGetTime();
  double curr_time;
  
  while (!window.is_closed()) {

    curr_time = glfwGetTime();

    if (last_state_id != client.get_state_id()) {
      state = client.get_last_state();
      static_cast<breakout::graphics::game_scene*>(scene)->set_state(&state);
      last_state_id = client.get_state_id();
    }

    input.handle(window.get_window(), state, /*TODO*/ 0, curr_time - last_time);
    state.update(curr_time - last_time);

    last_time = curr_time;     

    window.clear();
    scene = scene->render();
    window.update();
  }

  std::cout << "closing ....";

  client.stop();
  network_thread.join();
}