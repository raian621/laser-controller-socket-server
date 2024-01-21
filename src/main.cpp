#include <stdio.h>
#include <stdlib.h>

#include "server.h"

int main(int argc, char** argv) {
  char* sockPath;
  if (argc == 2) {
    sockPath = argv[1];
  } else if (argc == 1) {
    sockPath = "./socket";
  } else {
    fprintf(stderr, "Usage %s <unix-socket-path>\n", argv[0]);
    exit(1);
  }

  printf("%s\n", sockPath);
  int res = listenForControls(sockPath);

  if (res == -1) {
    perror("Server terminating due to error\n");
    exit(1);
  }
}
