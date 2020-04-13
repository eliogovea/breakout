#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <iostream>
#include <cstring>
#include <string>
#include <cstdint>
#include <system_error>
#include <algorithm>

#include <breakout/core/state.hpp>
#include <breakout/core/state_reader.hpp>

namespace breakout::network {

class handler {
public:
  explicit handler(std::string server_ip, int32_t server_port, int32_t listen_port);
  void send(const std::string& msg);
  void request_join();
  void listen();
  void stop();

  const int64_t get_state_id();
  const breakout::core::state get_last_state();
private:
  std::string server_ip_;
  int32_t server_port_;

  int32_t listen_port_;
  int32_t socket_; // socket for listening the server

  int64_t state_id_{0};
  breakout::core::state last_state_{};

  bool listening_{false};
};

const int64_t handler::get_state_id() {
  return state_id_;
}

const breakout::core::state handler::get_last_state() {
  // TODO use mutex to avoid race conditions ????
  return last_state_;
}

handler::handler(std::string server_ip, int32_t server_port, int32_t listen_port)
: server_ip_{server_ip}, server_port_{server_port}, listen_port_(listen_port) {
  socket_ = socket(AF_INET, SOCK_DGRAM, 0);
  if (socket_ < 0) {
    throw std::system_error(std::error_code() /*TODO*/,
                            "could not create a socket");
  }

  struct sockaddr_in server_addr_;
  memset(&server_addr_, 0, sizeof(server_addr_));
  server_addr_.sin_family = AF_INET;
  server_addr_.sin_addr.s_addr = inet_addr("127.0.0.1");
  server_addr_.sin_port = htons(listen_port);

  auto bind_result = bind(socket_, (const struct sockaddr *)&server_addr_,
                          sizeof(server_addr_));
  if (bind_result < 0) {
    throw std::system_error(std::error_code() /*TODO*/, "could not bind ...");
  }
}

void handler::send(const std::string& msg) {
  auto fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (socket < 0) {
    throw std::system_error(std::error_code(), "could not create a socket");
  }

  struct sockaddr_in server_addr_;
  memset(&server_addr_, 0, sizeof(server_addr_));
  server_addr_.sin_family = AF_INET;
  server_addr_.sin_addr.s_addr = inet_addr(server_ip_.c_str());
  server_addr_.sin_port = htons(server_port_);

  sendto(fd, msg.c_str(), msg.size(), MSG_CONFIRM,
         (struct sockaddr *)&server_addr_, sizeof(server_addr_));
}

void handler::request_join() {
  std::string sp = "";
  int32_t p = listen_port_;
  while (p > 0) {
    sp += '0' + (p % 10);
    p /= 10;
  }
  std::reverse(sp.begin(), sp.end());
  send("JOIN " + sp);
}

void handler::stop() {
  listening_ = false;
}

void handler::listen() {
  listening_ = true;
  while (listening_) {
    std::cout << "waiting for a message from the server ...\n";

    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    socklen_t client_addr_len = sizeof(client_addr);

    const int max_size = 512;
    char buffer_[max_size + 1];

    auto msg_size = recvfrom(socket_, buffer_, max_size, MSG_WAITALL,
                            (struct sockaddr *)&client_addr, &client_addr_len);

    // TODO check if it is the server !!!

    if (msg_size == -1) {
      std::cout << "error receiving a message\n";
      return;
    }

    std::cout << "new message from server: " << msg_size << " !!!!!!!!!!!!!!!!!!!!!!\n";
    buffer_[msg_size] = 0;

    std::cout << "message:\n";
    std::cout << buffer_ << "\n\n";

    // last_state_ = breakout::core::state_reader::read_from_string(std::string{buffer_});
    
    // MOCK for testing only
    std::cout << "generate mock state.";
    state_id_++;
    last_state_ = breakout::core::state_reader::mock();
  }
}

} // namespace breakout::client::network