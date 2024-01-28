#pragma once

#include <vector>

#include <game/Game.hpp>
#include <server/Player.hpp>
#include <server/ClientServer.hpp>

#include <server/connection_info.hpp>
#include <server/Player.hpp>

// class ClientServer;

class MainServer{
  int _socket;
  struct sockaddr_in _serverAddr;
  
  std::vector<std::shared_ptr<Game>> _games;
  std::vector<std::shared_ptr<connection_info>> _activeConnections;


  void loop();

  public:

    std::shared_ptr<connection_info> getConnectionInfo(pthread_t tid);

    std::vector<std::shared_ptr<connection_info>> getConnections();

    std::vector<game_info> getGamesInfo();

    std::shared_ptr<Game> getGame(int id); // checks in player ids and gameId
    std::shared_ptr<Game> createGame(game_info info);
    void deleteGame(int id); // checks in gameId and player ids
    void endConnection(pthread_t tid);
    
    MainServer();
    ~MainServer();

    void init();
};