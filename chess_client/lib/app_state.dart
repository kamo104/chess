
import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter/widgets.dart';

import 'game.dart';

class GameCreateRequest{
  ValueNotifier<bool> sendToServer = ValueNotifier(false);
  ValueNotifier<bool> accepted = ValueNotifier(false);
}

class GameJoinRequest{
  ValueNotifier<bool> sendToServer = ValueNotifier(false);
  ValueNotifier<int> accepted = ValueNotifier(-1);
}

class GameMoveRequest{
  ValueNotifier<bool> sendToServer = ValueNotifier(false);
  ValueNotifier<int> accepted = ValueNotifier(-1);
  int x = -1;
  int y = -1;
  int x2 = -1;
  int y2 = -1;
}

class OpponentMove{
  ValueNotifier<bool> newMove = ValueNotifier(false);
  int x = -1;
  int y = -1;
  int x2 = -1;
  int y2 = -1;
}

class GameSet extends ChangeNotifier{
  List<Game> games = [];

  void updateGames(List<Game> newGames) {
    games = newGames;
    notifyListeners();
  }

  void addGame(Game newGame) {
    bool found = false;
    for(Game g2 in games){
      if(newGame.id == g2.id){
        found = true;
      }
    }
    if(found == false){
      games.add(newGame);
      notifyListeners();
    }
  }

}

class AppState {
  ValueNotifier<String> username = ValueNotifier('');
  GameSet gamesNotifier = GameSet();

  Game? currentGame;
  int myId = -1;

  ValueNotifier<bool> gameStart = ValueNotifier(false);
  ValueNotifier<bool> opponentJoined = ValueNotifier(false);
  ValueNotifier<bool> gameEnd = ValueNotifier(false);

  ValueNotifier<bool> opponentLeft = ValueNotifier(false);
  ValueNotifier<bool> leaveRequest = ValueNotifier(false);

  ValueNotifier<bool> pingGameList = ValueNotifier(false);

  OpponentMove opponentMove = OpponentMove();

  GameCreateRequest gameCreateRequest = GameCreateRequest();
  GameJoinRequest gameJoinRequest = GameJoinRequest();
  GameMoveRequest gameMoveRequest = GameMoveRequest();

  void sendGameCreateRequest(){
    gameCreateRequest.sendToServer.value = true;
  }
}