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

#ifdef __cplusplus
extern "C" {
#endif

#include "chatlib.h"

#ifdef __cplusplus
}
#endif

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
      printf("Connected client fd=%d\n", fd);
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
