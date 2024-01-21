#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <errno.h>

#include "server.h"
#include "controller.h"

int listenForCommands(char* sockPath) {
  int serverSocket;
  if ((serverSocket = createServerSocket(sockPath)) == -1) {
    perror("error creating server socket");
    return -1;
  }

  initController();
  listen(serverSocket, 1);
  for (;;) {
    printf("Awaiting client connection...\n");

    int clientSocket;
    if ((clientSocket = accept(serverSocket, 0, 0)) == -1) {
      perror("error accepting connection to client");
      continue;
    }

    char buf[1024];
    int n;
    while ((n = recv(serverSocket, buf, sizeof(buf), 0)) > 0) {
      printf("%.1024s\n", buf);
    }



    close(clientSocket);
  }

  closeServerSocket(serverSocket);
  return 0; 
}

// returns file descriptor to a newly created Unix Domain Socket at path 
// `sockPath`
int createServerSocket(char *sockPath) {
  int serverSocket = socket(AF_UNIX, SOCK_STREAM, 0);
  if (serverSocket == -1) {
    printf("SOCKET ERROR\n");
    return -1;
  }

  struct sockaddr_un serverSockAddr;
  memset(&serverSockAddr, 0, sizeof(struct sockaddr_un));
  serverSockAddr.sun_family = AF_UNIX;
  strcpy(serverSockAddr.sun_path, sockPath); 
  
  int len = sizeof(serverSockAddr);

  if (unlink(sockPath) == -1 && errno != ENOENT) {
    perror("unlink error");
    close(serverSocket);
    return -1;
  }

  if (bind(serverSocket, (const struct sockaddr*)&serverSockAddr, len) == -1){
    printf("BIND ERROR\n");
    close(serverSocket);
    return -1;
  }

  return serverSocket;
}

int closeServerSocket(int serverSocket) {
  printf("closing the socket");
  if (close(serverSocket) == -1) {
    perror("close error");
    return -1;
  }

  return 0;
}