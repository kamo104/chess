import 'package:flutter/material.dart';

import 'app_state.dart';

class LoginPage extends StatelessWidget {
  const LoginPage(this.state, {super.key});

  final AppState state;

  @override
  Widget build(BuildContext context) {
    return PopScope(
      canPop: false,
      child: Scaffold(
        appBar: AppBar(
          backgroundColor: Theme.of(context).colorScheme.inversePrimary,
          title: const Text('Username Input'),
        ),
        body: Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: <Widget>[
              const Text(
                'Enter your username:',
              ),
              SizedBox(
                width: 200,              
                child: 
                  TextField(
                    autofocus: true,
                    onSubmitted: (value){
                      if(value == ''){
                        return;
                      }
                      state.username.value = value;
                      Navigator.pop(context);
                    },
                    decoration: const InputDecoration(
                      border: OutlineInputBorder(),
                    ),
                  ),
              ),
            ],
          ),
        ),
      )
    );
  }
}