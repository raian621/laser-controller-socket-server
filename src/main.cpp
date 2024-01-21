#include <stdio.h>
#include <stdlib.h>

#include "server.h"
#include "commands.h"

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
  // int res = listenForCommands(sockPath);

  // if (res == -1) {
  //   perror("Server terminating due to error\n");
  //   exit(1);
  // }data[0] = CONTROL_CMD;
  char data[12];
  data[1] = 'x';
  for (int i = 0; i < 4; i++) { data[i + 2] = (char)((int)1.2f >> (3-i) & 0xff); }
  data[5] = 'y';
  for (int i = 0; i < 4; i++) { data[i + 6] = (char)((int)3.4f >> (3-i) & 0xff); }
  data[9] = 's';
  data[10] = 0;
  data[11] = '\0';

  printf("%s\n", data);

  Command cmd;
  parseCommand(data, &cmd);
}
