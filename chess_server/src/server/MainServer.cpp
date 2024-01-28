#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <algorithm>
#include <memory>

#include <server/MainServer.hpp>
#include <server/network_message.hpp>
#include <game/Game.hpp>
#include <server/ClientServer.hpp>

void MainServer::deleteGame(int id){
  auto it = std::find_if(_games.begin(), _games.end(), [id](std::shared_ptr<Game> game){
    return game->info.id == id || game->info.opponent.user.id == id || game->info.owner.user.id == id;
  });
  if(it != _games.end()){
    _games.erase(it);
  }
}

std::shared_ptr<connection_info> MainServer::getConnectionInfo(pthread_t tid){
  auto it = std::find_if(_activeConnections.begin(), _activeConnections.end(), [tid](std::shared_ptr<connection_info> info){
    return info->tid == tid || info->user.id == tid;
  });
  if(it != _activeConnections.end()){
    return *it;
  }
  return nullptr;
}

std::vector<std::shared_ptr<connection_info>> MainServer::getConnections(){
  return _activeConnections;
}

std::vector<game_info> MainServer::getGamesInfo(){
  std::vector<game_info> gamesInfo;
  for(auto game : _games){
    gamesInfo.push_back(game->info);
  }
  return gamesInfo;
}

std::shared_ptr<Game> MainServer::getGame(int id){
  auto it = std::find_if(_games.begin(), _games.end(), [id](std::shared_ptr<Game> game){
    return game->info.id == id || game->info.opponent.user.id == id || game->info.owner.user.id == id;
  });
  if(it != _games.end()){
    return *it;
  }
  return nullptr;
}

std::shared_ptr<Game> MainServer::createGame(game_info info){
  std::shared_ptr<Game> game = std::make_shared<Game>(info);
  _games.push_back(game);
  return game;
}

void MainServer::endConnection(pthread_t tid){
  auto it = std::find_if(_activeConnections.begin(), _activeConnections.end(), [tid](std::shared_ptr<connection_info> info){
    return info->tid == tid || info->user.id == tid;
  });
  if(it != _activeConnections.end()){
    _activeConnections.erase(it);
  }
}

void MainServer::loop(){
  
  socklen_t addr_size = sizeof(sockaddr_storage);
  sockaddr_storage address;

  int clientSocket = accept(_socket, (struct sockaddr *) &address, &addr_size);


  std::shared_ptr<connection_info> info = std::make_shared<connection_info>();
  info->client_socket = clientSocket;
  info->address = address;
  info->address_size = addr_size;
  info->status = connection_status::CONNECTED;

  _activeConnections.push_back(info);
  std::shared_ptr<ClientServer> clientServer = std::make_shared<ClientServer>(info, this);
  info->server = clientServer;

  clientServer->init(this);
}

void MainServer::init(){
  bind(_socket, (struct sockaddr *) &_serverAddr, sizeof(_serverAddr));
  
  if(listen(_socket,10)!=0){
    printf("Error\n");
    return;
  }

  printf("Listening\n");
  while(1){
    loop();
  }
}

MainServer::MainServer(){
  _socket = socket(PF_INET, SOCK_STREAM, 0);
  const int enable = 1;
  setsockopt(_socket,SOL_SOCKET,SO_REUSEADDR,&enable,sizeof(int));
  _serverAddr.sin_family = AF_INET;
  _serverAddr.sin_port = htons(9999);
  _serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  memset(_serverAddr.sin_zero, '\0', sizeof(_serverAddr.sin_zero));
}

MainServer::~MainServer(){
  close(_socket);
}