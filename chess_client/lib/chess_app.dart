import 'package:flutter/material.dart';

import 'app_state.dart';
// import 'login_page.dart';
import 'home_page.dart';

class ChessApp extends StatelessWidget {
  const ChessApp(this.state,{super.key});

  final AppState state;

  @override
  Widget build(BuildContext context) {
    return MaterialApp(

      title: 'Chess',
      themeMode: ThemeMode.dark, 
      darkTheme: ThemeData(
        brightness: Brightness.dark,
      ),

      home: HomePage(state, title: 'Chess'),
    );
  }
}