#pragma once

#include <cstdint>
#include <exception>
#include <system_error>

#include <cstring>

#include <string>
#include <deque>
#include <vector>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>


namespace breakout::server {

class server {
public:
  explicit server();
  void run();
  void add_client(const uint32_t ip);
  void remove_client(const uint32_t ip);
private:
  int32_t socket_;
  std::vector<uint32_t> clients_ip{};
  std::deque<std::string> messages_{};
};

server::server() {
  socket_ = socket(AF_INET, SOCK_DGRAM, 0);
  if (socket_ < 0) {
    throw std::system_error(std::error_code() /*TODO*/,
                            "could not create a socket");
  }

  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(1515);

  auto bind_result = bind(socket_, (const struct sockaddr *)&addr,
                          sizeof(addr));
  
  if (bind_result < 0) {
    throw std::system_error(std::error_code() /*TODO*/, "could not bind ...");
  }
}

void server::run() {
  // TODO
  while (true) {
  }
}

}