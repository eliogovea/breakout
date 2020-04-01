#pragma once

#include <cstdint>

namespace breakout::client::network {

class handler {
public:
  explicit handler(int32_t server_ip);
private:
  uint32_t socket_;
};

} // namespace breakout::client::network