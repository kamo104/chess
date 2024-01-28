#include <algorithm>

#include "game/pieces/PieceInfo.hpp"
#include "game/pieces/Piece.hpp"
#include "game/pieces/Bishop.hpp"

#include "game/Board.hpp"

#include "game/Move.hpp"

bool Bishop::is_checking_king(){

    Piece* king = _board->get_enemy_king(info.color);

    int king_x = king->info.position.x;
    int king_y = king->info.position.y;
    int i = info.position.x;
    int j = info.position.y;

    return can_move(Move(i,j,king_x,king_y,PieceType::EMPTY));
}

bool Bishop::can_move(Move move){
    int x = move.x;
    int y = move.y;
    int x2 = move.x2;
    int y2 = move.y2;

    if(abs(x-x2) != abs(y-y2)){
        return false;
    }

    if(x > x2 && y > y2){
        for(int i = 1; i < x - x2; i++){
            if(_board->get_piece(x - i, y - i)->info.type != PieceType::EMPTY){
                return false;
            }
        }
    }
    else if(x > x2 && y < y2){
        for(int i = 1; i < x - x2; i++){
            if(_board->get_piece(x - i, y + i)->info.type != PieceType::EMPTY){
                return false;
            }
        }
    }
    else if(x < x2 && y > y2){
        for(int i = 1; i < x2 - x; i++){
            if(_board->get_piece(x + i, y - i)->info.type != PieceType::EMPTY){
                return false;
            }
        }
    }
    else if(x < x2 && y < y2){
        for(int i = 1; i < x2 - x; i++){
            if(_board->get_piece(x + i, y + i)->info.type != PieceType::EMPTY){
                return false;
            }
        }
    }

    return true;
}