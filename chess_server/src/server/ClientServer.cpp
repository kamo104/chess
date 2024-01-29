
#include <sys/socket.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <algorithm>
#include <memory>

#include <server/connection_info.hpp>
#include <server/ClientServer.hpp>
#include <server/network_message.hpp>

// MAY ERROR
#include <server/MainServer.hpp>

void ClientServer::sendGamesList(std::shared_ptr<connection_info> connection){
    char buffer[2000];
    buffer[0] = (char)ServerMessageType::LIST_OF_GAMES_INFO;
    int offset = 1;
    for(auto game : _mainServer->getGamesInfo()){
        if(game.id == 0)
            continue;

        unsigned int gameId = htonl(game.id);
        memcpy(buffer+offset, &gameId, sizeof(game.id));
        offset += 4;
        unsigned int ownerId = htonl(game.owner.user.id);
        memcpy(buffer+offset, &ownerId, sizeof(game.owner.user.id));
        offset += 4;
        memcpy(buffer+offset, &game.owner.user.name, 32);
        offset += 32;
        memcpy(buffer+offset, &game.owner.color, 1);
        offset += 4;
        unsigned int opponentId = htonl(game.opponent.user.id);
        memcpy(buffer+offset, &opponentId, sizeof(game.opponent.user.id));
        offset += 4;
        memcpy(buffer+offset, &game.opponent.user.name, 32);
        offset += 32;
        memcpy(buffer+offset, &game.opponent.color, 1);
        offset += 4;
        memcpy(buffer+offset, &game.current_turn, 1);
        offset += 4;
    }
    send(connection->client_socket, buffer, offset, 0);
}

void ClientServer::onGetGamesList(){
    printf("get list of games\n");
    sendGamesList(_connection);
}

void ClientServer::onSetPlayerName(char* message, int length){
    printf("set player name\n");
    char name[32];
    memcpy(name, message, length);
    bzero(_connection->user.name,32);
    memcpy(_connection->user.name, name, length);
    _connection->user.id = rand() % unsigned(-1);

    char data[5];
    data[0] = (char)ServerMessageType::PLAYER_ID;
    unsigned int id = htonl(_connection->user.id);
    memcpy(data+1, &id, sizeof(_connection->user.id));
    send(_connection->client_socket, (void*)data, 5, 0);
}

void ClientServer::onJoinGame(char* message, int length){
    unsigned int gameId = ntohl(*(unsigned int*)message);
    // unsigned int game_id = int((unsigned char)(message[0]) << 24 |
    //         (unsigned char)(message[1]) << 16 |
    //         (unsigned char)(message[2]) << 8 |
    //         (unsigned char)(message[3]));

    printf("chosen game_id: %d\n",gameId);
    auto game = _mainServer->getGame(gameId);

    if(game == NULL){
        printf("game not found %d\n",gameId);
        char data[1] = {(char)ServerMessageType::GAME_NOT_JOINED};
        send(_connection->client_socket, (void*)data, 1, 0);
        return;
    }

    if(game->info.opponent.user.id != 0 && game->info.owner.user.id != 0){
        printf("game is full\n");
        char data[1] = {(char)ServerMessageType::GAME_NOT_JOINED};
        send(_connection->client_socket, (void*)data, 1, 0);
        return;
    }
    game->info.opponent = player_info(_connection->user, game->info.owner.color == PieceColor::WHITE ? PieceColor::BLACK : PieceColor::WHITE);

    printf("sending game joined\n");
    char data[9];
    data[0] = (char)ServerMessageType::GAME_JOINED;
    unsigned int id = htonl(game->info.id);
    memcpy(data+1, &id, sizeof(game->info.id));
    data[5] = (char)game->info.opponent.color;
    send(_connection->client_socket, (void*)data, 9, 0);

    printf("sending player joined your game\n");
    char data2[sizeof(player_info)+1];
    data2[0] = (char)ServerMessageType::PLAYER_JOINED_YOUR_GAME;
    unsigned int ownerId = htonl(game->info.owner.user.id);
    int offset = 1;
    memcpy(data2+offset, &ownerId, sizeof(ownerId));
    offset += sizeof(ownerId);
    memcpy(data2+offset, &game->info.opponent.user.name, 32);
    offset += 32;
    memcpy(data2+offset, &game->info.opponent.color, 1);
    offset += 4;
    send(_mainServer->getConnectionInfo(game->info.owner.user.id)->client_socket, (void*)data2, offset, 0);
    
    printf("sending game started\n");
    char data3[1];
    data3[0] = (char)ServerMessageType::GAME_STARTED;
    send(_mainServer->getConnectionInfo(game->info.owner.user.id)->client_socket, (void*)data3, 1, 0);
    send(_connection->client_socket, (void*)data3, 1, 0);

    printf("sending game list to all\n");
    for(auto connection : _mainServer->getConnections()){
        sendGamesList(connection);
    }
}

void ClientServer::onCreateGame(char* message, int length){
    printf("create new game\n");
    game_info gameInfo;
    gameInfo.id = _connection->user.id;
    gameInfo.owner = player_info(_connection->user, rand() % 2 == 0 ? PieceColor::WHITE : PieceColor::BLACK);
    gameInfo.opponent = player_info(user_info(), PieceColor::NONE);
    gameInfo.current_turn = PieceColor::WHITE;

    auto game = _mainServer->createGame(gameInfo);
    if(game == NULL){
        printf("game not created\n");
        char data[1] = {(char)ServerMessageType::GAME_NOT_CREATED};
        send(_connection->client_socket, (void*)data, 1, 0);
        return;
    }
    printf("game created\n");
    char data[6];
    data[0] = (char)ServerMessageType::GAME_CREATED;
    unsigned int id = htonl(game->info.id);
    memcpy(data+1, &id, sizeof(game->info.id));
    data[5] = (char)game->info.owner.color;
    send(_connection->client_socket, (void*)data, 6, 0);

    printf("sending game list to all\n");
    for(auto connection : _mainServer->getConnections()){
        sendGamesList(connection);
    }
}

void ClientServer::onMovePiece(char* message, int length){
    printf("move piece\n");
    unsigned int x = message[0];
    unsigned int y = message[1];
    unsigned int x2 = message[2];
    unsigned int y2 = message[3];

    auto game = _mainServer->getGame(_connection->user.id);
    if(game == NULL){
        printf("game not found\n");
        char data[1] ={(char)ServerMessageType::PIECE_MOVE_REJECT};
        send(_connection->client_socket, (void*)data, 1, 0);
        return;
    }
    if(game->info.opponent.user.id == 0 || game->info.owner.user.id == 0){
        printf("game not full\n");
        char data[1] ={(char)ServerMessageType::PIECE_MOVE_REJECT};
        send(_connection->client_socket, (void*)data, 1, 0);
        return;
    }
    PieceColor playerColor = game->info.owner.user.id == _connection->user.id ? game->info.owner.color : game->info.opponent.color;
    GameStatus status = game->move(playerColor,Move(x,y,x2,y2));
    if(status == GameStatus::ILLEGAL_MOVE){
        printf("sending piece move reject\n");
        char data[1] ={(char)ServerMessageType::PIECE_MOVE_REJECT};
        send(_connection->client_socket, (void*)data, 1, 0);
        return;
    }

    int my_socket = _connection->client_socket;
    int opponent_socket = game->info.owner.user.id == _connection->user.id ? _mainServer->getConnectionInfo(game->info.opponent.user.id)->client_socket : _mainServer->getConnectionInfo(game->info.owner.user.id)->client_socket;
    
    printf("sending piece move confirm\n");
    char data[1] = {ServerMessageType::PIECE_MOVE_CONFIRM};
    send(my_socket, (void*)data, 1, 0);

    printf("sending piece move\n");
    char data2[5] = {ServerMessageType::ENEMY_PIECE_MOVE,(char)x,(char)y,(char)x2,(char)y2};
    send(opponent_socket, (void*)data2, 5, 0);

    if(status == GameStatus::DRAW){
        playerColor = PieceColor::NONE;
    }
    if(status == GameStatus::WHITE_WON || status == GameStatus::BLACK_WON || status == GameStatus::DRAW){
        printf("sending game ended\n");
        char data3[2] = {ServerMessageType::GAME_ENDED, (char)playerColor};
        usleep(1000000);
        send(my_socket, (void*)data3, 2, 0);
        send(opponent_socket, (void*)data3, 2, 0);
    }
}

void ClientServer::onLeaveGame(){
    printf("leave game\n");
    auto game = _mainServer->getGame(_connection->user.id);
    if(game == NULL){
        printf("game not found\n");
        return;
    }
    player_info opponentInfo = game->info.owner.user.id == _connection->user.id ? game->info.opponent : game->info.owner;
    if(opponentInfo.user.id != 0){
        printf("sending player left your game\n");
        char data[1];
        data[0] = (char)ServerMessageType::PLAYER_LEFT_YOUR_GAME;

        send(_mainServer->getConnectionInfo(opponentInfo.user.id)->client_socket, (void*)data, 1, 0);
    }

    _mainServer->deleteGame(game->info.id);

    printf("sending game list to all\n");
    for(auto connection : _mainServer->getConnections()){
        sendGamesList(connection);
    }
}


void ClientServer::parse_message(char* message, int length){
    ClientMessageType type = (ClientMessageType)message[0];
    switch(type){
        case ClientMessageType::GET_LIST_OF_GAMES:{
            onGetGamesList();
            break;
        }
        case ClientMessageType::SET_PLAYER_NAME:{
            onSetPlayerName(message+1, length-1);
            break;
        }
        case ClientMessageType::JOIN_GAME:{
            onJoinGame(message+1, length-1);
            break;
        }
        case ClientMessageType::CREATE_NEW_GAME:{
            onCreateGame(message+1, length-1);
            break;
        }
        case ClientMessageType::MOVE_PIECE:{
            onMovePiece(message+1, length-1);
            break;
        }
        case ClientMessageType::LEAVE_GAME:{
            onLeaveGame();
            break;
        }
    }
}

void* ClientServer::socket_thread(void* arg){
    auto connection = (connection_info*)arg;
    auto server = connection->server;

    #define BUFFER_LEN 2000
    char buffer[BUFFER_LEN];
    int length;
    int flag = 1; 
    setsockopt(connection->client_socket, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));
    while((length = recv(connection->client_socket, buffer, BUFFER_LEN, 0)) > 0){
        server->parse_message(buffer, length);
        bzero(buffer, BUFFER_LEN);
    }

    printf("connection closed\n");
    std::shared_ptr<Game> game = server->_mainServer->getGame(connection->user.id);
    if(game == NULL){
        printf("game not found\n");
        connection->server->_mainServer->endConnection(connection->tid);
        pthread_exit(NULL);
    }
    player_info opponentInfo = game->info.owner.user.id == connection->user.id ? game->info.opponent : game->info.owner;
    if(opponentInfo.user.id != 0){
        printf("sending player left your game\n");
        char data[1];
        data[0] = (char)ServerMessageType::PLAYER_LEFT_YOUR_GAME;

        send(server->_mainServer->getConnectionInfo(opponentInfo.user.id)->client_socket, (void*)data, 1, 0);
    }

    connection->server->_mainServer->deleteGame(connection->user.id);
    connection->server->_mainServer->endConnection(connection->tid);


    pthread_exit(NULL);
}

void ClientServer::init(MainServer* mainServer){
    pthread_create(&_connection->tid, NULL, socket_thread, _connection.get());
    pthread_detach(_connection->tid);
}
