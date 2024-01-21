#ifndef SERVER_H
#define SERVER_H

int listenForCommands(char*);
int createServerSocket(char*);
int closeServerSocket(int);

#endif