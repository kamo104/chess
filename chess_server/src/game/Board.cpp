
#include <memory>

#include "game/Board.hpp"

#include "game/pieces/Pawn.hpp"
#include "game/pieces/Rook.hpp"
#include "game/pieces/Knight.hpp"
#include "game/pieces/Bishop.hpp"
#include "game/pieces/Queen.hpp"
#include "game/pieces/King.hpp"
#include "game/pieces/Piece.hpp"



Board::Board(){
}

// void Board::init(std::shared_ptr<Board> board){
void Board::init(Board* board){
    self = board;
    
    for(int i = 2; i < 6; i++){
        for(int j = 0; j < 8; j++){
            _board[i][j] = new Piece(self, piece_info(PieceType::EMPTY, NONE,PiecePosition(i,j)));
            // _board[i][j] = std::make_shared<Piece>(self, piece_info(PieceType::EMPTY, NONE,PiecePosition(i,j)));
        }
    }
    for(int i = 0; i < 8; i++){
        _board[1][i] = new Pawn(self, piece_info(PieceType::PAWN, WHITE,PiecePosition(i,1)));
        _board[6][i] = new Pawn(self, piece_info(PieceType::PAWN, BLACK,PiecePosition(i,6)));

    }
    _board[0][0] = new Rook(self, piece_info(PieceType::ROOK, WHITE,PiecePosition(0,0)));
    _board[0][1] = new Knight(self, piece_info(PieceType::KNIGHT, WHITE,PiecePosition(0,1)));
    _board[0][2] = new Bishop(self, piece_info(PieceType::BISHOP, WHITE,PiecePosition(0,2)));
    _board[0][3] = new King(self, piece_info(PieceType::KING, WHITE,PiecePosition(0,3)));
    _board[0][4] = new Queen(self, piece_info(PieceType::QUEEN, WHITE,PiecePosition(0,4)));
    _board[0][5] = new Bishop(self, piece_info(PieceType::BISHOP, WHITE,PiecePosition(0,5)));
    _board[0][6] = new Knight(self, piece_info(PieceType::KNIGHT, WHITE,PiecePosition(0,6)));
    _board[0][7] = new Rook(self, piece_info(PieceType::ROOK, WHITE,PiecePosition(0,7)));
    


    _board[7][0] = new Rook(self, piece_info(PieceType::ROOK, BLACK,PiecePosition(7,0)));
    _board[7][1] = new Knight(self, piece_info(PieceType::KNIGHT, BLACK,PiecePosition(7,1)));
    _board[7][2] = new Bishop(self, piece_info(PieceType::BISHOP, BLACK,PiecePosition(7,2)));
    _board[7][3] = new King(self, piece_info(PieceType::KING, BLACK,PiecePosition(7,3)));
    _board[7][4] = new Queen(self, piece_info(PieceType::QUEEN, BLACK,PiecePosition(7,4)));
    _board[7][5] = new Bishop(self, piece_info(PieceType::BISHOP, BLACK,PiecePosition(7,5)));
    _board[7][6] = new Knight(self, piece_info(PieceType::KNIGHT, BLACK,PiecePosition(7,6)));
    _board[7][7] = new Rook(self, piece_info(PieceType::ROOK, BLACK,PiecePosition(7,7)));
}

Board::~Board(){
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            delete _board[i][j];
        }
    }
}

Piece* Board::get_piece(char x, char y) const{
    return _board[x][y];
}

Piece* Board::get_king(PieceColor color) const{
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            Piece* piece = _board[i][j];
            if(piece->info.color == color && piece->info.type == PieceType::KING){
                return piece;
            }
        }
    }
    return nullptr;
}
Piece* Board::get_enemy_king(PieceColor color) const{
    PieceColor enemy = color == WHITE ? BLACK : WHITE;
    
    return get_king(enemy);
}

bool Board::is_enemy(char x, char y, Piece* piece) const{
    if(_board[x][y]->info.type == PieceType::EMPTY){
        return false;
    }
    return _board[x][y]->info.color != piece->info.color;
}

GameStatus Board::check_after_move(bool player_color, Move move) {
    char x = move.x;
    char y = move.y;
    char x2 = move.x2;
    char y2 = move.y2;
    PieceType promotion = move.promotion;

    Piece* piece0 = _board[x][y];
    Piece* piece1 = _board[x2][y2];

    _board[x][y] = new Piece(self, piece_info(PieceType::EMPTY, PieceColor(2),PiecePosition(x,y)));
    _board[x2][y2] = piece0;
    piece0->info.position.x = x2;
    piece0->info.position.y = y2;

    bool check = false;
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            if(_board[i][j]->info.color != player_color && _board[i][j]->is_checking_king()){
                check = true;
                break;
            }
        }
    }

    GameStatus status = check ? player_color ? GameStatus::WHITE_CHECKED : GameStatus::BLACK_CHECKED : GameStatus::ONGOING;
    

    delete _board[x][y];
    _board[x][y] = piece0;
    piece0->info.position.x = x;
    piece0->info.position.y = y;
    _board[x2][y2] = piece1;
    return status;
}

bool Board::is_legal_move(PieceColor player_color, Move move){
    char x = move.x;
    char y = move.y;
    char x2 = move.x2;
    char y2 = move.y2;
    PieceType promotion = move.promotion;

    Piece* piece0 = _board[x][y];
    Piece* piece1 = _board[x2][y2];


    if(x < 0 || x > 7 || y < 0 || y > 7 || x2 < 0 || x2 > 7 || y2 < 0 || y2 > 7){
        return false;
    }
    if(piece0->info.color != player_color){
        return false;
    }
    if(x==x2 && y==y2){
        return false;
    }
    if(_board[x][y]->info.type == PieceType::EMPTY){
        return false;
    }
    if(_board[x][y]->info.color == _board[x2][y2]->info.color){
        return false;
    }
    if(piece0->can_move(move) == false){
        return false;
    }
    switch(check_after_move(player_color, move)){
        case(GameStatus::ILLEGAL_MOVE):
            return false;
        case(GameStatus::WHITE_CHECKED):
            if(player_color == WHITE){
                return false;
            }
            break;
        case(GameStatus::BLACK_CHECKED):
            if(player_color == BLACK){
                return false;
            }
            break;
    }
    

    return true;
}

GameStatus Board::check_check(PieceColor player_color){
    Piece* king = get_king(player_color);
    int king_x = king->info.position.x;
    int king_y = king->info.position.y;

    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            Piece* piece = _board[i][j];
            if(piece->info.color != player_color && piece->is_checking_king()){
                return player_color == WHITE ? GameStatus::WHITE_CHECKED : GameStatus::BLACK_CHECKED;
            }
        }
    }
    return GameStatus::ONGOING;
}

GameStatus Board::move(PieceColor player_color, Move move){
    GameStatus status = GameStatus::ONGOING;
    if(is_legal_move(player_color, move) == false){
        return GameStatus::ILLEGAL_MOVE;
    }

    char x = move.x;
    char y = move.y;
    char x2 = move.x2;
    char y2 = move.y2;
    PieceType promotion = move.promotion;

    Piece* piece0 = _board[x][y];
    Piece* piece1 = _board[x2][y2];

    if(promotion == EMPTY){
        delete piece1;
        _board[x2][y2] = piece0;
        piece0->info.position.x = x2;
        piece0->info.position.y = y2;
        _board[x][y] = new Piece(self, piece_info(PieceType::EMPTY, PieceColor(2),PiecePosition(x,y)));
    }

    bool color = piece0->info.color;
    if((color == WHITE && x2 == 7) || (color == BLACK && x2 == 0)){
        switch(promotion){
            case(ROOK):
                _board[x2][y2] = new Rook(self, piece_info(PieceType::ROOK, PieceColor(color),PiecePosition(x2,y2)));
                break;
            case(KNIGHT):
                _board[x2][y2] = new Knight(self, piece_info(PieceType::KNIGHT, PieceColor(color),PiecePosition(x2,y2)));
                break;
            case(BISHOP):
                _board[x2][y2] = new Bishop(self, piece_info(PieceType::BISHOP, PieceColor(color),PiecePosition(x2,y2)));
                break;
            case(QUEEN):
                _board[x2][y2] = new Queen(self, piece_info(PieceType::QUEEN, PieceColor(color),PiecePosition(x2,y2)));
                break;
            case(PAWN):
                _board[x2][y2] = new Pawn(self, piece_info(PieceType::PAWN, PieceColor(color),PiecePosition(x2,y2)));
                break;
            case(EMPTY):
                _board[x2][y2] = new Queen(self, piece_info(PieceType::QUEEN, PieceColor(color),PiecePosition(x2,y2)));
                break;
        }
        delete piece1;
        delete piece0;
        _board[x][y] = new Piece(self, piece_info(PieceType::EMPTY, PieceColor(2),PiecePosition(x,y)));
    }

    // check for checkmate
    // try every move for every piece
    PieceColor enemy_color = player_color == WHITE ? BLACK : WHITE;
    GameStatus check = check_check(enemy_color);

    if(check == GameStatus::ONGOING){
        return status;
    }
    
    status = enemy_color == BLACK ? GameStatus::WHITE_WON : GameStatus::BLACK_WON;
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            Piece* movingPiece = _board[i][j];
            if(movingPiece->info.color != enemy_color){
                continue;
            }
            
            for(int k=0; k<8; k++){
                for(int l=0; l<8; l++){
                    Move move = Move(i,j,k,l,PieceType::EMPTY);
                    if(is_legal_move(enemy_color, move)){
                        Piece* tmpPiece = _board[k][l];

                        // mock move
                        _board[k][l] = movingPiece;
                        movingPiece->info.position.x = k;
                        movingPiece->info.position.y = l;
                        _board[i][j] = new Piece(self, piece_info(PieceType::EMPTY, PieceColor::NONE,PiecePosition(i,j)));

                        status = check_check(enemy_color);

                        // undo mock move
                        delete _board[i][j];
                        _board[i][j] = movingPiece;
                        movingPiece->info.position.x = i;
                        movingPiece->info.position.y = j;
                        _board[k][l] = tmpPiece;

                        if(status == GameStatus::ONGOING){
                            return status;
                        }
                        
                        
                    }
                }
            }
        }
    }

    return status;
}