#include <gtest/gtest.h>
#include <thread>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#include "server.h"

TEST(SocketTests, WriteToRawUnixSocket) {
  char message[] = "ligma";
  int serverSock = createServerSocket("socket");
  listen(serverSock, 1);
  
  std::thread serverThread([](int serverSock) {
    int clientSock;
    if ((clientSock = accept(serverSock, 0, 0)) == -1) {
      perror("accept error");
      close(serverSock);
      FAIL();
    }

    char buf[256];
    int n = recv(clientSock, buf, sizeof(buf), 0);

    ASSERT_EQ(strcmp(buf, "ligma"), 0);
  }, serverSock);

  int dialSocket;
  if ((dialSocket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    perror("client socket error");
    close(serverSock);
    FAIL();
  }

  struct sockaddr_un serverAddr;
  memset(&serverAddr, 0, sizeof(struct sockaddr_un)); 
  serverAddr.sun_family = AF_UNIX;
  strcpy(serverAddr.sun_path, "socket");

  if (connect(dialSocket, (const struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
    perror("client can't connect to server");
    close(serverSock);
    close(dialSocket);
    FAIL();
  }

  if (send(dialSocket, message, sizeof(message), 0) == -1) {
    perror("error sending message");
    close(serverSock);
    close(dialSocket);
    FAIL();
  }

  close(dialSocket);
  serverThread.join();
}