import 'dart:typed_data';

import 'package:chess_client/player.dart';
import 'package:flutter/material.dart';

import 'dart:convert';
import 'dart:io';

import 'chess_app.dart';
import 'app_state.dart';
import 'game.dart';
import 'network_message.dart';





void main() async {
  final socket = await RawSocket.connect("localhost", 9999);

  AppState state = AppState();

  void parseData(Uint8List data){
  switch (ServerMessageType.values[data[0]]) {
    case ServerMessageType.LIST_OF_GAMES_INFO:
      print("got games list");
      Uint8List gameData = data.sublist(1);
      List<Game> games = [];
      int offset = 0;
      for(int i = 0; i < gameData.length; i += offset){
        offset = 0;
        int gameId = gameData.sublist(i,i+4).buffer.asByteData().getUint32(0);
        offset += 4;
        int ownerId = gameData.sublist(i+offset,i+offset+4).buffer.asByteData().getUint32(0);
        offset += 4;
        String ownerName = ascii.decode(gameData.sublist(i+offset,i+offset+32)).replaceAll('\u0000', '');
        offset += 32;
        bool ownerColor = gameData[i+offset] == 1;
        offset += 4;
        int opponentId = gameData.sublist(i+offset,i+offset+4).buffer.asByteData().getUint32(0);
        offset += 4;
        String opponentName = ascii.decode(gameData.sublist(i+offset,i+offset+32)).replaceAll('\u0000', '');
        offset += 32;
        bool opponentColor = gameData[i+offset] == 1;
        offset += 4;
        bool currentTurn = gameData[i+offset] == 1;
        offset += 4;

        Game game = Game(gameId,Player(ownerId,ownerName,ownerColor),Player(opponentId,opponentName,opponentColor));
        game.currentTurn = currentTurn;

        games.add(game);
      }
      for(var game in games){
        print(game.id);
      }
      state.gamesNotifier.updateGames(games);
      break;
    case ServerMessageType.GAME_JOINED:
      int gameId = data.sublist(1,5).buffer.asByteData().getUint32(0);
      print("game joined id:$gameId");

      bool myColor = data[5] == 1;
      state.currentGame = state.gamesNotifier.games.firstWhere((element) => element.id == gameId);
      state.currentGame!.opponent = Player(state.myId,state.username.value,myColor);

      state.gameJoinRequest.accepted.value = 1;
      break;
    case ServerMessageType.GAME_NOT_JOINED:
      print("game not joined");
      state.gameJoinRequest.accepted.value = 0;
      break;
    case ServerMessageType.PLAYER_JOINED_YOUR_GAME:
      Uint8List gameData = data.sublist(1);
      int offset = 0;
      int opponentId = gameData.sublist(0,4).buffer.asByteData().getUint32(0);
      offset += 4;
      String opponentName = ascii.decode(gameData.sublist(offset,offset+32)).replaceAll('\u0000', '');
      offset += 32;
      bool opponentColor = gameData[offset] == 1;
      offset += 4;

      state.currentGame!.opponent = Player(opponentId,opponentName,opponentColor);
      // event for starting the game
      state.opponentJoined.value = true;
      print("player joined your game id:$opponentId");
      break;
    case ServerMessageType.GAME_STARTED:
      state.gameStart.value = true;
      print("game started");
      break;
    case ServerMessageType.GAME_ENDED:
      Uint8List gameData = data.sublist(1);
      int status = gameData.buffer.asByteData().getUint8(0);
      state.currentGame!.status = GameStatus.values[status];
      state.gameEnd.value = true;
      print("game ended");
      break;
    case ServerMessageType.GAME_CREATED:
      // game created
      int gameId = data.sublist(1,5).buffer.asByteData().getUint32(0);
      print("created game with id: $gameId");
      bool myColor = data[5] == 1;
      
      state.currentGame = Game(gameId,Player(state.myId,state.username.value,myColor),Player(-1,'',!myColor));

      state.gamesNotifier.addGame(state.currentGame!);

      state.gameCreateRequest.accepted.value = true;
      break;
    case ServerMessageType.GAME_NOT_CREATED:
      print("game not created");
      state.gameCreateRequest.accepted.value = false;
      break;
    case ServerMessageType.ENEMY_PIECE_MOVE:
      print("got enemy move");
      state.opponentMove.x = data[1];
      state.opponentMove.y = data[2];
      state.opponentMove.x2 = data[3];
      state.opponentMove.y2 = data[4];

      state.currentGame!.currentTurn = !state.currentGame!.currentTurn;
      state.opponentMove.newMove.value = true;
      if(data.length == 7){
        state.currentGame!.status = GameStatus.values[data[6]];
        state.gameEnd.value = true;
      }
      break;
    case ServerMessageType.PIECE_MOVE_CONFIRM:
      print("got move confirm");
      state.currentGame!.currentTurn = !state.currentGame!.currentTurn;
      state.gameMoveRequest.accepted.value = 1;
      if(data.length == 2){
        state.currentGame!.status = GameStatus.values[data[3]];
        state.gameEnd.value = true;
      }
      break;
    case ServerMessageType.PIECE_MOVE_REJECT:
      print("got move reject");
      state.gameMoveRequest.accepted.value = 0;
      break;
    case ServerMessageType.PLAYER_ID:
      state.myId = data.sublist(1,5).buffer.asByteData().getInt32(0);
      print("got player id:${state.myId}");
    case ServerMessageType.PLAYER_LEFT_YOUR_GAME:
      print("player left your game");
      if(state.currentGame!.status != GameStatus.ONGOING) break;
      state.currentGame!.opponent = Player(-1,'',false);
      state.opponentLeft.value = true;
      break;
    default:
      print(ascii.decode(data));
  }
}

  socket.listen((event) {
    switch (event) {
      case RawSocketEvent.read:
        final data = socket.read();
        if(data == null) break;
        parseData(data);
        break;
      case RawSocketEvent.write:
        socket.writeEventsEnabled = false;
        break;
      case RawSocketEvent.readClosed:
        socket.close();
        break;
      case RawSocketEvent.closed:
        break;
      default:
        throw "Unexpected event $event";
    }
  });

  // ClientMessageType.CREATE_NEW_GAME
  state.gameCreateRequest.sendToServer.addListener(() { 
    if(state.gameCreateRequest.sendToServer.value == true){
      state.gameCreateRequest.sendToServer.value = false;

      print("sending game create request");
      // send game request
      Uint8List data = Uint8List(1);
      data[0] = ClientMessageType.CREATE_NEW_GAME.index;
      socket.write(data);
    }
  });
  
  // ClientMessageType.JOIN_GAME
  state.gameJoinRequest.sendToServer.addListener(() { 
    if(state.gameJoinRequest.sendToServer.value == true){
      state.gameJoinRequest.sendToServer.value = false;

      print("sending game join request");
      // send game request
      var data = ByteData(5);
      data.setUint8(0, ClientMessageType.JOIN_GAME.index);
      data.setUint32(1, state.currentGame!.id);
      socket.write(data.buffer.asUint8List());
    }
  });

  // ClientMessageType.SET_PLAYER_NAME
  state.username.addListener(() {
    print("sending username");
    // send username
    Uint8List data = Uint8List(33);
    data[0] = ClientMessageType.SET_PLAYER_NAME.index;
    data.setAll(1,ascii.encode(state.username.value));
    socket.write(data);
  });

  // ClientMessageType.MOVE_PIECE
  state.gameMoveRequest.sendToServer.addListener((){
    if(state.gameMoveRequest.sendToServer.value == true){
      state.gameMoveRequest.sendToServer.value = false;

      print("sending move request");
      // send move request
      Uint8List data = Uint8List(5);
      data[0] = ClientMessageType.MOVE_PIECE.index;
      data[1] = state.gameMoveRequest.x;
      data[2] = state.gameMoveRequest.y;
      data[3] = state.gameMoveRequest.x2;
      data[4] = state.gameMoveRequest.y2;
      socket.write(data);
    }
  
  });

  // ClientMessageType.LEAVE_GAME
  state.leaveRequest.addListener(() {
    if(state.leaveRequest.value == true){
      state.leaveRequest.value = false;

      print("sending leave game request");
      // send leave game request
      Uint8List data = Uint8List(1);
      data[0] = ClientMessageType.LEAVE_GAME.index;
      socket.write(data);
    }
  });

  // ClientMessageType.GET_LIST_OF_GAMES
  state.pingGameList.addListener(() {
    if(state.pingGameList.value == true){
      state.pingGameList.value = false;

      print("asking for games list");
      Uint8List data = Uint8List(1);
      data[0] = ClientMessageType.GET_LIST_OF_GAMES.index;
      socket.write(data);
    }
  });

  state.pingGameList.value = true;

  runApp(ChessApp(state));
}