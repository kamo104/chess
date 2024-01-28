#include <algorithm>

#include "game/pieces/PieceInfo.hpp"
#include "game/pieces/Piece.hpp"
#include "game/pieces/Pawn.hpp"

#include "game/Board.hpp"

#include "game/Move.hpp"

bool Pawn::is_checking_king(){

    Piece* king = _board->get_enemy_king(info.color);

    int king_x = king->info.position.x;
    int king_y = king->info.position.y;
    int x = info.position.x;
    int y = info.position.y;

    if(info.color == WHITE){
        if(x == king_x+1 && y == king_y+1)
            return true;
        if(x == king_x+1 && y == king_y-1)
            return true;
    }
    else{
        if(x == king_x+1 && y == king_y+1)
            return true;
        if(x == king_x+1 && y == king_y-1)
            return true;
    }

    return false;
}

bool Pawn::can_move(Move move){
    int x = move.x;
    int y = move.y;
    int x2 = move.x2;
    int y2 = move.y2;

    if(x == x2){
        return false;
    }

    if(info.color == WHITE){
        if(x2 == x + 1 && _board->get_piece(x2, y2)->info.type == PieceType::EMPTY){
            return true;
        }
        if(x == 1 && x2 == x + 2 && _board->get_piece(x2, y2)->info.type == PieceType::EMPTY){
            return true;
        }
        if(_board->is_enemy(x2, y2, this) && x2 == x + 1 && abs(y-y2)==1){
            return true;
        }
    }
    else{
        if(x2 == x - 1 && _board->get_piece(x2, y2)->info.type == PieceType::EMPTY){
            return true;
        }
        if(x == 6 && x2 == x - 2 && _board->get_piece(x2, y2)->info.type == PieceType::EMPTY){
            return true;
        }
        if(_board->is_enemy(x2, y2, this) && x2 == x - 1 && abs(y-y2)==1){
            return true;
        }
    }
    
    return false;
}