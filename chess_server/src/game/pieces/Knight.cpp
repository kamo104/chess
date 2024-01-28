#include "game/pieces/PieceInfo.hpp"
#include "game/pieces/Piece.hpp"
#include "game/pieces/Knight.hpp"

#include "game/Board.hpp"

#include "game/Move.hpp"

bool Knight::is_checking_king(){

    Piece* king = _board->get_enemy_king(info.color);

    int king_x = king->info.position.x;
    int king_y = king->info.position.y;
    int x = info.position.x;
    int y = info.position.y;

    return can_move(Move(x,y,king_x,king_y,PieceType::EMPTY));
}

bool Knight::can_move(Move move){
    int x = move.x;
    int y = move.y;
    int x2 = move.x2;
    int y2 = move.y2;

    if(x == x2 && y == y2){
        return false;
    }
    if(x2 > x + 2 || x2 < x - 2 || y2 > y + 2 || y2 < y - 2){
        return false;
    }
    if(x2 == x + 2 || x2 == x - 2){
        if(y2 == y + 1 || y2 == y - 1){
            return true;
        }
    }
    if(y2 == y + 2 || y2 == y - 2){
        if(x2 == x + 1 || x2 == x - 1){
            return true;
        }
    }

    return false;
}