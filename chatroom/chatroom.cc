#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cassert>

namespace {
  constexpr auto kMaxClients = 1000;
  constexpr auto kServerPort = 8888;  // 聊天服务器端口
}

typedef struct Client {
  int fd;
  char *nickname;
} Client;

typedef struct ChatState {
  int server_sock;
  int num_clients;
  int max_client_fd;
  Client *clients[kMaxClients];
} ChatState;

ChatState *chatroom = nullptr;

int CreateTCPServer(int port) {
  int sock_fd;
  if ((sock_fd = ::socket(AF_INET, SOCK_STREAM, 0)) == -1) {
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
  sa.sin_port = htons(kServerPort);
  sa.sin_addr.s_addr = htonl(INADDR_ANY);

  ret = ::bind(sock_fd, (struct sockaddr*)&sa, sizeof(sa));
  if (ret == -1) {
    perror("Bind tcp server socket failed");
    return -1;
  }

  ret = ::listen(sock_fd, 511);
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

    cli_sock = ::accept(server_sock, (struct sockaddr*)&sa, &sock_len);
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

void *ChatMalloc(size_t size) {
  void *ptr = malloc(size);
  if (ptr == nullptr) {
    perror("Out of memory");
    exit(1);
  }

  return ptr;
}

void *ChatRealloc(void *ptr, size_t size) {
  void *new_ptr = realloc(ptr, size);
  if (new_ptr == nullptr) {
    perror("Out of memory");
    exit(1);
  }

  ptr = new_ptr;

  return ptr;
}

Client *CreateClient(int fd) {
  assert(chatroom->clients[fd] == nullptr);

  Client *client = static_cast<Client*>(ChatMalloc(sizeof(*client)));
  SetSocketNonBlockNoDelay(fd);
  client->fd = fd;

  char nickname[20];
  int nickname_len = snprintf(nickname, sizeof(nickname), "user:%d", fd);
  client->nickname = static_cast<char*>(ChatMalloc(nickname_len));
  memcpy(client->nickname, nickname, nickname_len);

  chatroom->clients[client->fd] = client;

  if (client->fd > chatroom->max_client_fd) {
    chatroom->max_client_fd = client->fd;
  }
  ++chatroom->num_clients;

  return client;
}

void FreeClient(Client *client) {
  free(client->nickname);
  ::shutdown(client->fd, SHUT_RDWR);
  ::close(client->fd);

  chatroom->clients[client->fd] = nullptr;
  --chatroom->num_clients;
  if (client->fd == chatroom->max_client_fd) {
    int i = 0;
    for (i = chatroom->max_client_fd; i >= 0; --i) {
      if (chatroom->clients[i] != nullptr) {
        chatroom->max_client_fd = i;
        break;
      }

    }

    if (i == -1) {
      chatroom->max_client_fd = -1;
    }
  }

  free(client);
}

void InitChatRoom() {
  chatroom = static_cast<ChatState*>(ChatMalloc(sizeof(*chatroom)));
  memset(chatroom, 0, sizeof(*chatroom));

  chatroom->max_client_fd = -1;
  chatroom->num_clients = 0;
  chatroom->server_sock = CreateTCPServer(kServerPort);
  if (chatroom->server_sock == -1) {
    exit(1);
  }
}

void FreeChatRoom() {
  for (int i = 0; i < chatroom->max_client_fd; ++i) {
    if (chatroom->clients[i] != nullptr) {
      FreeClient(chatroom->clients[i]);
    }
  }

  free(chatroom);
}

void SendMsgToAllClientBut(int excluded_fd, char *msg, size_t msg_len) {
  for (int i = 0; i <= chatroom->max_client_fd; ++i) {
    if (chatroom->clients[i] == nullptr ||
        i == excluded_fd) {
      continue;
    }

    write(i, msg, msg_len);
  }
}

bool stopped = false;

void SigHandler(int sig) {
  stopped = true;
}

int main(int argc, char **argv) {
  InitChatRoom();

  struct sigaction sig_action;
  sig_action.sa_handler = SigHandler;
  sigaction(SIGINT, &sig_action, nullptr);
  sigaction(SIGTERM, &sig_action, nullptr);
  
  while (!stopped) {
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(chatroom->server_sock, &readfds);

    for (int i = 0; i <= chatroom->max_client_fd; ++i) {
      if (chatroom->clients[i] == nullptr) {
        continue;
      }

      FD_SET(i, &readfds);
    }

    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    int max_fd = chatroom->max_client_fd;
    if (max_fd < chatroom->server_sock) {
      max_fd = chatroom->server_sock;
    }

    int ret = select(max_fd + 1, &readfds, nullptr, nullptr, &tv);
    if (ret == -1) {
      if (errno == EINTR) {
        printf("interrputed by SIGINT\n");
        break;
      }
      perror("Select failed");
      exit(1);
    }

    if (ret == 0) {
      continue;
    }

    if (FD_ISSET(chatroom->server_sock, &readfds)) {
      int fd = AcceptClient(chatroom->server_sock);
      Client *client = CreateClient(fd);
      const char *welcome_msg =
        "Welcome to Chatroom! "
        "Use /nick <nickname> to set your nickname.\n";
      write(client->fd, welcome_msg, strlen(welcome_msg));
      printf("Connected client fd=%d", fd);
    }

    char read_buf[256];
    for (int i = 0; i <= chatroom->max_client_fd; ++i) {
      if (chatroom->clients[i] == nullptr) {
        continue;
      }

      if (FD_ISSET(i, &readfds)) {
        int read_size = read(i, read_buf, sizeof(read_buf));

        if (read_size <= 0) {
          printf("Disconnected client fd=%d, nickname=%s",
            i, chatroom->clients[i]->nickname);
          FreeClient(chatroom->clients[i]);
          continue;
        }

        Client *client = chatroom->clients[i];
        read_buf[read_size] = 0;

        if (read_buf[0] == '/') {
          char *p;
          p = strchr(read_buf, '\r');
          if (p) {
            *p = 0;
          }

          p = strchr(read_buf, '\n');
          if (p) {
            *p = 0;
          }

          char *arg = strchr(read_buf, ' ');
          if (arg) {
            *arg = 0;
            ++arg;
          }

          if (strcmp(read_buf, "/nick") == 0 && arg) {
            free(client->nickname);
            int nickname_len = strlen(arg);
            client->nickname = static_cast<char*>(ChatMalloc(nickname_len));
            memcpy(client->nickname, arg, nickname_len + 1);
          } else {
            const char *err_msg = "Unsupported command\n";
            write(i, err_msg, strlen(err_msg));
          }

          continue;
        }

        char msg[256];
        int msg_len =
            snprintf(msg, sizeof(msg), "%s> %s", client->nickname, read_buf);
        printf("%s", msg);

        SendMsgToAllClientBut(i, msg, msg_len);
      }
    }
  }

  printf("good bye\n");

  FreeChatRoom();
  return 0;
}
