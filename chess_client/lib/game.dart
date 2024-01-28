import "player.dart";

import 'package:flutter/material.dart';
import 'dart:io';

enum PieceType{
  EMPTY,
  WHITE_PAWN,
  WHITE_ROOK,
  WHITE_KNIGHT,
  WHITE_BISHOP,
  WHITE_QUEEN,
  WHITE_KING,
  BLACK_PAWN,
  BLACK_ROOK,
  BLACK_KNIGHT,
  BLACK_BISHOP,
  BLACK_QUEEN,
  BLACK_KING
}

enum GameStatus{
  BLACK_WON,
  WHITE_WON,
  DRAW,
  ONGOING
}


Map<PieceType,Image> pieceImages = {
  PieceType.BLACK_BISHOP:Image(image: FileImage(File.fromUri(Uri.parse("./img/black_bishop.png")))),
  PieceType.BLACK_KING:Image(image: FileImage(File.fromUri(Uri.parse("./img/black_king.png")))),
  PieceType.BLACK_KNIGHT:Image(image: FileImage(File.fromUri(Uri.parse("./img/black_knight.png")))),
  PieceType.BLACK_PAWN:Image(image: FileImage(File.fromUri(Uri.parse("./img/black_pawn.png")))),
  PieceType.BLACK_QUEEN:Image(image: FileImage(File.fromUri(Uri.parse("./img/black_queen.png")))),
  PieceType.BLACK_ROOK:Image(image: FileImage(File.fromUri(Uri.parse("./img/black_rook.png")))),
  PieceType.WHITE_BISHOP:Image(image: FileImage(File.fromUri(Uri.parse("./img/white_bishop.png")))),
  PieceType.WHITE_KING:Image(image: FileImage(File.fromUri(Uri.parse("./img/white_king.png")))),
  PieceType.WHITE_KNIGHT:Image(image: FileImage(File.fromUri(Uri.parse("./img/white_knight.png")))),
  PieceType.WHITE_PAWN:Image(image: FileImage(File.fromUri(Uri.parse("./img/white_pawn.png")))),
  PieceType.WHITE_QUEEN:Image(image: FileImage(File.fromUri(Uri.parse("./img/white_queen.png")))),
  PieceType.WHITE_ROOK:Image(image: FileImage(File.fromUri(Uri.parse("./img/white_rook.png")))),
  PieceType.EMPTY:Image(image: FileImage(File.fromUri(Uri.parse("./img/empty.png")))),
};



class Game{
  List<List<PieceType>> board = [];
  Game(this.id, this.owner, this.opponent){
    board = [
  [PieceType.WHITE_ROOK,PieceType.WHITE_KNIGHT,PieceType.WHITE_BISHOP,PieceType.WHITE_KING,PieceType.WHITE_QUEEN,PieceType.WHITE_BISHOP,PieceType.WHITE_KNIGHT,PieceType.WHITE_ROOK],
  [PieceType.WHITE_PAWN,PieceType.WHITE_PAWN,PieceType.WHITE_PAWN,PieceType.WHITE_PAWN,PieceType.WHITE_PAWN,PieceType.WHITE_PAWN,PieceType.WHITE_PAWN,PieceType.WHITE_PAWN,],
  [PieceType.EMPTY,PieceType.EMPTY,PieceType.EMPTY,PieceType.EMPTY,PieceType.EMPTY,PieceType.EMPTY,PieceType.EMPTY,PieceType.EMPTY,],
  [PieceType.EMPTY,PieceType.EMPTY,PieceType.EMPTY,PieceType.EMPTY,PieceType.EMPTY,PieceType.EMPTY,PieceType.EMPTY,PieceType.EMPTY,],
  [PieceType.EMPTY,PieceType.EMPTY,PieceType.EMPTY,PieceType.EMPTY,PieceType.EMPTY,PieceType.EMPTY,PieceType.EMPTY,PieceType.EMPTY,],
  [PieceType.EMPTY,PieceType.EMPTY,PieceType.EMPTY,PieceType.EMPTY,PieceType.EMPTY,PieceType.EMPTY,PieceType.EMPTY,PieceType.EMPTY,],
  [PieceType.BLACK_PAWN,PieceType.BLACK_PAWN,PieceType.BLACK_PAWN,PieceType.BLACK_PAWN,PieceType.BLACK_PAWN,PieceType.BLACK_PAWN,PieceType.BLACK_PAWN,PieceType.BLACK_PAWN,],
  [PieceType.BLACK_ROOK,PieceType.BLACK_KNIGHT,PieceType.BLACK_BISHOP,PieceType.BLACK_KING,PieceType.BLACK_QUEEN,PieceType.BLACK_BISHOP,PieceType.BLACK_KNIGHT,PieceType.BLACK_ROOK],
];
  }

  int id;

  Player owner;
  Player opponent;
  bool currentTurn = true;

  GameStatus status = GameStatus.ONGOING;
}