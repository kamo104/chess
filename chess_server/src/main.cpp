#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <pthread.h>

#include <server/MainServer.hpp>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


int main(){
  printf("pid: %d\n", getpid());
  
  MainServer server;
  server.init();
  
  return 0;
}