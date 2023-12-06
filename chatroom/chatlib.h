#ifndef CHATROOM_CHATLIB_H_
#define CHATROOM_CHATLIB_H_

#include <stddef.h>

int CreateTCPServer(int port);
int SetSocketNonBlockNoDelay(int fd);
int AcceptClient(int server_socket);
int TCPConnect(const char *addr, int port, int nonblock);

void *ChatMalloc(size_t size);
void *ChatRealloc(void *ptr, size_t size);

#endif // CHATROOM_CHATLIB_H_
