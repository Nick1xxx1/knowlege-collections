#define _POSIX_C_SOURCE 200112L
#include "chatlib.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int CreateTCPServer(int port) {
  int sock_fd;
  if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("Create tcp server socket failed");
    return -1;
  }

  int yes = 1;
  int ret = setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  if (ret == -1) {
    perror("Set tcp server socket reuse addr opt failed");
    return -1;
  }

  struct sockaddr_in sa;
  memset(&sa, 0, sizeof(sa));
  sa.sin_family = AF_INET;
  sa.sin_port = htons(port);
  sa.sin_addr.s_addr = htonl(INADDR_ANY);

  ret = bind(sock_fd, (struct sockaddr*)&sa, sizeof(sa));
  if (ret == -1) {
    perror("Bind tcp server socket failed");
    return -1;
  }

  ret = listen(sock_fd, 511);
  if (ret == -1) {
    perror("Listen tcp server socket failed");
    return -1;
  }

  return sock_fd;
}

int SetSocketNonBlockNoDelay(int fd) {
  int flags = fcntl(fd, F_GETFL);
  if (flags == -1) {
    perror("Get file descriptor flags failed");
    return -1;
  }

  int ret = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
  if (ret == -1) {
    perror("Set file descriptor nonblock failed");
    return -1;
  }

  int yes = 1;
  ret = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes));
  if (ret == -1) {
    perror("Set tcp server socket no delay opt failed");
    return -1;
  }

  return 0;
}

int AcceptClient(int server_sock) {
  int cli_sock;
  while (1) {
    struct sockaddr_in sa;
    socklen_t sock_len = sizeof(sa);

    cli_sock = accept(server_sock, (struct sockaddr*)&sa, &sock_len);
    if (cli_sock == -1) {
      if (errno == EINTR || errno == EAGAIN) {
        continue;
      }

      perror("Accept client socket failed");
      return -1;
    }

    break;
  }

  return cli_sock;
}

int TCPConnect(const char *addr, int port, int nonblock) {
  struct addrinfo hints;
  char port_str[6];
  snprintf(port_str, sizeof(port_str), "%d", port);
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  struct addrinfo *server_info;
  if (getaddrinfo(addr, port_str, &hints, &server_info) != 0) {
    return -1;
  }

  struct addrinfo *p;
  int sock;
  int ret = -1;
  for (p = server_info; p != NULL; p = p->ai_next) {
    if ((sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      continue;
    }

    if (nonblock && SetSocketNonBlockNoDelay(sock) == -1) {
      close(sock);
      break;
    }

    if (connect(sock, p->ai_addr, p->ai_addrlen) == -1) {
      if (errno == EINPROGRESS && nonblock) {
        return sock;
      }

      close(sock);
      break;
    }

    ret = sock;
    break;
  }

  freeaddrinfo(server_info);
  return ret;
}

void *ChatMalloc(size_t size) {
  void *ptr = malloc(size);
  if (ptr == NULL) {
    perror("Out of memory");
    exit(1);
  }

  return ptr;
}

void *ChatRealloc(void *ptr, size_t size) {
  void *new_ptr = realloc(ptr, size);
  if (new_ptr == NULL) {
    perror("Out of memory");
    exit(1);
  }

  ptr = new_ptr;

  return ptr;
}


