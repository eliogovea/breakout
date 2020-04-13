#include <breakout/network/server.hpp>

int main() {
  breakout::network::server server{};
  server.run();
}