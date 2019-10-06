#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "socket.h"


#ifndef PORT
  #define PORT 50001
#endif
#define BUF_SIZE 128

int main(void) {
    int sock_fd = connect_to_server(PORT, "127.0.0.1");
    // client connected to the server now ask it for a username
    char username[BUF_SIZE + 1];
    int num_read = read(STDIN_FILENO, username, BUF_SIZE);     // reading from stdin
    if (num_read == 0) {
      close(sock_fd); // ALWAYS NEED A USERNAME OTHERWISE EXIT
      exit(1);
    }

    username[num_read] = '\0';
    // write the username to the server
    int num_written = write(sock_fd, username, num_read);  // write to the server
    if (num_written != num_read) {
        perror("client: write");
        close(sock_fd);
        exit(1);
    }

    // sends any text it receives from stdin to the server
    // and prints any text it receives from the server on stdout

    // Read input from the user, send it to the server, and then accept the
    // echo that returns. Exit when stdin is closed.
    char buf[BUF_SIZE + 1];

    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);
    FD_SET(sock_fd, &read_fds);
    int maxfd;

    while (1) {
      FD_ZERO(&read_fds);
      FD_SET(STDIN_FILENO, &read_fds);
      FD_SET(sock_fd, &read_fds);

      if (STDIN_FILENO > sock_fd) {
          maxfd = STDIN_FILENO + 1;
      } else {
          maxfd = sock_fd + 1;
      }

      if (select(maxfd, &read_fds, NULL, NULL, NULL) == -1) {
          perror("select");
          exit(1);
      }

      if (FD_ISSET(STDIN_FILENO, &read_fds)) {
        int num_read = read(STDIN_FILENO, buf, BUF_SIZE);     // reading from stdin
        if (num_read == 0) {
            break;
        }
        buf[num_read] = '\0';         // Just because I'm paranoid

        int num_written = write(sock_fd, buf, num_read);  // write to the server
        if (num_written != num_read) {
            perror("client: write");
            close(sock_fd);
            exit(1);
        }
      }

      if (FD_ISSET(sock_fd, &read_fds)) {
        num_read = read(sock_fd, buf, BUF_SIZE);    // accept the echo that returns back from server
        buf[num_read] = '\0';
        printf("%s", buf);
      }
    }

    close(sock_fd);
    return 0;
}
