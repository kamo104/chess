#pragma once

#include <memory>

#include <server/connection_info.hpp>
// #include <server/MainServer.hpp>

class MainServer;

class ClientServer{
    std::shared_ptr<connection_info> _connection;

    MainServer *_mainServer;

    void parse_message(char *message, int length);

    void onGetGamesList();
    void onSetPlayerName(char *message, int length);
    void onJoinGame(char *message, int length);
    void onCreateGame(char *message, int length);
    void onMovePiece(char *message, int length);
    void onLeaveGame();

    void sendGamesList(std::shared_ptr<connection_info> connection);

    public:

        ClientServer(std::shared_ptr<connection_info> connection, MainServer *mainServer) : _connection(connection), _mainServer(mainServer){};
        ~ClientServer(){};

        void init(MainServer* mainServer);

        static void *socket_thread(void *arg);


};