#include <errno.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifdef __cplusplus
extern "C" {
#endif

#include "chatlib.h"

#ifdef __cplusplus
}
#endif

void DisableRawModeAtExit();

int SetRawMode(int fd, int enable) {
  static int rawmode_is_set = 0;
  static struct termios orig_termios;
  if (enable == 0) {
    if (rawmode_is_set && tcsetattr(fd, TCSAFLUSH, &orig_termios) != -1) {
      rawmode_is_set = 0;
    }

    return 0;
  }

  static int atexit_registered = 0;
  struct termios raw;
  if (!isatty(fd)) {
    goto fatal;
  }

  if (!atexit_registered) {
    atexit(DisableRawModeAtExit);
    atexit_registered = 1;
  }

  if (tcgetattr(fd, &orig_termios) == -1) {
    goto fatal;
  }

  raw = orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN);
  raw.c_cc[VMIN] = 1;
  raw.c_cc[VTIME] = 0;

  if (tcsetattr(fd, TCSAFLUSH, &raw) < 0) {
    goto fatal;
  }

  rawmode_is_set = 1;
  return 0;

fatal:
  errno = ENOTTY;

  return -1;
}

void DisableRawModeAtExit() {
  SetRawMode(STDIN_FILENO, 0);
}

void CleanTerminalCurrentLine() {
  write(fileno(stdout), "\e[2K", 4);
}

void MoveTerminalCursorToLineStart() {
  write(fileno(stdout), "\r", 1);
}

constexpr int kIBMaxLen = 128;
typedef struct InputBuffer {
  char buf[kIBMaxLen];
  int len;
} InputBuffer;

enum {
  kIBErr = 0,
  kIBOk,
  kIBGotLine
};

int AppendInputBuffer(InputBuffer *ib, int ch) {
  if (ib->len > kIBMaxLen) {
    return kIBErr;
  }

  ib->buf[ib->len] = ch;
  ++ib->len;

  return kIBOk;
}

void HideInputBuffer(InputBuffer *ib);
void ShowInputBuffer(InputBuffer *ib);

int FeedInputBufferChar(InputBuffer *ib, int ch) {
  switch (ch)
  {
  case '\n':
    break;
  case '\r':
    return kIBGotLine;
  case 127: // 空格
    if (ib->len > 0) {
      --ib->len;
      HideInputBuffer(ib);
      ShowInputBuffer(ib);
    }
    break;
  default:
    if (AppendInputBuffer(ib, ch) == kIBOk) {
      write(fileno(stdout), ib->buf + ib->len - 1, 1);
    }
    break;
  }

  return kIBOk;
}

void HideInputBuffer(InputBuffer *ib) {
  (void)ib;
  CleanTerminalCurrentLine();
  MoveTerminalCursorToLineStart();
}

void ShowInputBuffer(InputBuffer *ib) {
  write(fileno(stdout), ib->buf, ib->len);
}

void ClearInputBuffer(InputBuffer *ib) {
  ib->len = 0;
  HideInputBuffer(ib);
}


int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Usage: %s <host> <port>\n", argv[0]);
    exit(1);
  }

  int sock = TCPConnect(argv[1], atoi(argv[2]), 0);
  if (sock == -1) {
    perror("Failed to connect to server");
    exit(1);
  }

  int stdin_fd = fileno(stdin);
  int stdout_fd = fileno(stdout);
  SetRawMode(stdin_fd, 1);

  fd_set read_fds;
  InputBuffer ib;
  ClearInputBuffer(&ib);

  while (1) {
    FD_ZERO(&read_fds);
    FD_SET(sock, &read_fds);
    FD_SET(stdin_fd, &read_fds);
    int max_fd = sock > stdin_fd ? sock : stdin_fd;

    int num_events = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
    if (num_events == -1) {
      perror("Select error");
      exit(1);
    }

    char buf[128];
    if (FD_ISSET(sock, &read_fds)) {
      ssize_t count = read(sock, buf, sizeof(buf));
      if (count <= 0) {
        printf("Connection lost\n");
        exit(1);
      }

      HideInputBuffer(&ib);
      write(stdout_fd, buf, count);
      ShowInputBuffer(&ib);
    } else if (FD_ISSET(stdin_fd, &read_fds)) {
      ssize_t count = read(stdin_fd, buf, sizeof(buf));
      for (int i = 0; i < count; ++i) {
        int ret = FeedInputBufferChar(&ib, buf[i]);
        switch (ret)
        {
        case kIBGotLine:
          AppendInputBuffer(&ib, '\n');
          HideInputBuffer(&ib);
          write(stdout_fd, "you> ", 5);
          write(stdout_fd, ib.buf, ib.len);
          write(sock, ib.buf, ib.len);
          ClearInputBuffer(&ib);
          break;
        case kIBOk:
          break;
        default:
          break;
        }
      }
    }
  }

  close(sock);

  return 0;
}
