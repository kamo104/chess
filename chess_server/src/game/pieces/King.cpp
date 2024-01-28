#include <algorithm>

#include "game/pieces/PieceInfo.hpp"
#include "game/pieces/Piece.hpp"
#include "game/pieces/King.hpp"

#include "game/Board.hpp"

#include "game/Move.hpp"

bool King::is_checking_king(){
    return false;
}

bool King::can_move(Move move){
    int x = move.x;
    int y = move.y;
    int x2 = move.x2;
    int y2 = move.y2;

    if(x == x2 && y == y2){
        return false;
    }
    if(abs(x2 - x) > 1 || abs(y2 - y) > 1){
        return false;
    }
    if(_board->get_piece(x2, y2)->info.color == info.color){
        return false;
    }

    return true;
}