#pragma once

#include <iostream>

#include <tuple>
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


namespace breakout::network {

enum class message_type {join, command, leave};

class server {
public:
  explicit server();
  void run();
  void handle_connection();
  std::string handle_message(const char* msg, const std::size_t size);
private:
  int32_t socket_;

  std::vector<std::tuple<int32_t, int32_t>> clients_;
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

void server::handle_connection() {
  std::cout << "waiting for a client ...\n";

  struct sockaddr_in client_addr;
  memset(&client_addr, 0, sizeof(client_addr));
  socklen_t client_addr_len = sizeof(client_addr);

  const std::size_t max_size = 512;
  char buffer_[max_size + 1];

  auto msg_size = recvfrom(socket_, buffer_, max_size, MSG_WAITALL,
                           (struct sockaddr *)&client_addr, &client_addr_len);

  std::cout << "client: " << client_addr.sin_addr.s_addr << "\n";

  

  if (msg_size == -1) {
    return;
  }

  buffer_[msg_size] = 0;
  std::cout << "message received: " << buffer_ << "\n";

  if (std::string{buffer_}.substr(0, 4) == "JOIN") {
    int32_t port = 0;
    for (int i = 5; i < msg_size; i++) {
      port = 10 * port + buffer_[i] - '0';
    }
    
    clients_.push_back({static_cast<int32_t>(client_addr.sin_addr.s_addr), port});
    client_addr.sin_port = htons(port);
    // TODO send when the game is about to start
    std::cout << "sending response, port: " << port << "\n";
    sendto(socket_, "OK", 2, MSG_CONFIRM,
         (struct sockaddr *)&client_addr, client_addr_len);

  } else if (std::string{buffer_}.substr(0, 7) == "COMMAND") {
    // TODO
  }
  
  // TODO handle message
  // TODO doit properly !!!

  return;
}

std::string server::handle_message(const char* msg, const std::size_t size) {
  // TODO
}

void server::run() {
  // TODO
  while (true) {
    handle_connection(); // blocking, just for now (testing)
  }
}

}