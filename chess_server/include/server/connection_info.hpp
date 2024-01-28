#pragma once

#include <pthread.h>
#include <bits/socket.h>
#include <memory>
#include <server/Player.hpp>

class ClientServer;

enum class connection_status{
  CONNECTED,
  DISCONNECTED
};


struct connection_info{
  pthread_t tid=0;
  int client_socket;
  sockaddr_storage address;
  socklen_t address_size;
  connection_status status;
  user_info user;
  std::shared_ptr<ClientServer> server;
};