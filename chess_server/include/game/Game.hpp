#pragma once

#include <server/Player.hpp>
#include <game/pieces/PieceInfo.hpp>
#include <game/Move.hpp>
#include <game/Board.hpp>

#include <memory>

struct game_info
{
    game_info(){};
    game_info(player_info owner, unsigned int id) : owner(owner), id(id){};
    unsigned int id=0;
    player_info owner;
    player_info opponent;
    PieceColor current_turn = PieceColor::WHITE;
};

class Game{

    // std::shared_ptr<Board> _board = std::make_shared<Board>();
    Board* _board;

    public:
        game_info info;

        Game(game_info gameinfo);
        Game();

        GameStatus move(PieceColor playerColor, Move move);
};