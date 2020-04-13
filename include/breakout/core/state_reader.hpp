#pragma once

#include <fstream>

#include <breakout/core/state.hpp>

namespace breakout::core {

class state_reader {
public:

  static state read_from_string(const std::string text) {
    // TODO
  }

  static state read_from_file(const std::string path) {
    std::ifstream input(path.data());
    // TODO    
  }

  static state mock() {
    return {
      2.0,
      2.0,
      {
        {{0, -1.0}, 1.0, 0.4, 0.1, 1}
      }, // paddles
      {
        {{0, 0}, 0.1, {0.3, 1}, 1}
      }, // balls
      {
        {{0, 0.7}, 0.3, 0.1, 2}, {{-0.3, 0.7}, 0.3, 0.1, 2}
      } // bricks
    };
  }

  // TODO read_from_network(network_handler handler);
};


}