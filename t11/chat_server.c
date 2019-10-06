#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "socket.h"


#ifndef PORT
  #define PORT 50001
#endif
#define MAX_BACKLOG 5
#define MAX_CONNECTIONS 4
#define BUF_SIZE 128


struct sockname {
    int sock_fd;
    char *username;
};


/* Accept a connection. Note that a new file descriptor is created for
 * communication with the client. The initial socket descriptor is used
 * to accept connections, but the new socket is used to communicate.
 * Return the new client's file descriptor or -1 on error.
 */
int setup_new_client(int fd, struct sockname *usernames) {
    int user_index = 0;
    while (user_index < MAX_CONNECTIONS && usernames[user_index].sock_fd != -1) {
        user_index++;
    }

    int client_fd = accept_connection(fd);  // accept a client
    if (client_fd < 0) {
        return -1;
    }

    if (user_index >= MAX_CONNECTIONS) {
        fprintf(stderr, "server: max concurrent connections\n");
        close(client_fd);
        return -1;
    }

    // read the username immediately and set the username
    char buf[BUF_SIZE + 1];
    int num_read = read(client_fd, &buf, BUF_SIZE);
    buf[num_read-1] = '\0';

    // username will always be provided
    usernames[user_index].username = (char*)malloc(sizeof(strlen(buf)+1));
    strncpy(usernames[user_index].username,buf,strlen(buf)+1);
    usernames[user_index].sock_fd = client_fd;      //ADD THE NEW CLIENT
    return client_fd;
}


/* Read a message from client_index and echo it back to them.
 * Return the fd if it has been closed or 0 otherwise.
 */
int read_from(int client_index, struct sockname *usernames) {
    int fd = usernames[client_index].sock_fd;       // get this clients fd
    char buf[BUF_SIZE + 1];

    int num_read = read(fd, &buf, BUF_SIZE);       // read what this client has to say
    buf[num_read] = '\0';

    char new_buf[BUF_SIZE + 1];
    snprintf(new_buf, BUF_SIZE+1, "%s: %s",usernames[client_index].username, buf);

    for (int index = 0; index < MAX_CONNECTIONS; index++){
      if(usernames[index].sock_fd != -1 && num_read != 0){
        if(write(usernames[index].sock_fd, new_buf, strlen(new_buf)) != strlen(new_buf)){ // write error or read nothing
            usernames[index].sock_fd = -1;
            return usernames[index].sock_fd;
        }
      }
      if(num_read == 0){
        usernames[index].sock_fd = -1;
        return usernames[index].sock_fd;
      }
    }

    return 0;                                 // echo sucsessfull
}


int main(void) {
    // This line causes stdout not to be buffered.
    // Don't change this! Necessary for autotesting.
    setbuf(stdout, NULL);

    struct sockaddr_in *self = init_server_addr(PORT);
    int sock_fd = setup_server_socket(self, MAX_BACKLOG);     // Server Setup

    // Create a list of chat client users.
    struct sockname usernames[MAX_CONNECTIONS];               // create an array of client structs
    for (int index = 0; index < MAX_CONNECTIONS; index++) {
        usernames[index].sock_fd = -1;                        // initally set them all to null
        usernames[index].username = NULL;
    }

    // The client accept - message accept loop. First, we prepare to listen to multiple
    // file descriptors by initializing a set of file descriptors.
    int max_fd = sock_fd;
    fd_set all_fds, listen_fds;              // set all fds
    FD_ZERO(&all_fds);
    FD_SET(sock_fd, &all_fds);              // add the server to all_fds

    while (1) {
        // select updates the fd_set it receives, so we always use a copy and retain the original.
        listen_fds = all_fds;               // copy of original
        int nready = select(max_fd + 1, &listen_fds, NULL, NULL, NULL);     // call select
        if (nready == -1) {
            perror("server: select");
            exit(1);
        }

        // Is it the original socket? Create a new connection ...
        if (FD_ISSET(sock_fd, &listen_fds)) {
            int client_fd = setup_new_client(sock_fd, usernames);
            if (client_fd < 0) {
                continue;
            }
            if (client_fd > max_fd) {
                max_fd = client_fd;
            }
            FD_SET(client_fd, &all_fds);          // after creating a new connection, add the new client to master fds
            printf("Accepted connection\n");
        }

        // Next, check the clients.
        // NOTE: We could do some tricks with nready to terminate this loop early.
        for (int index = 0; index < MAX_CONNECTIONS; index++) {
            if (usernames[index].sock_fd > -1 && FD_ISSET(usernames[index].sock_fd, &listen_fds)) {   // if the client has something to say
                // Note: never reduces max_fd
                int client_closed = read_from(index, usernames);          // read and echo back
                if (client_closed > 0) {
                    FD_CLR(client_closed, &all_fds);                     // REMOVE THAT FD
                    close(client_closed);
                    printf("Client %d disconnected\n", client_closed);  // CLIENT DISCCONECTED
                } else {
                    printf("Echoing message from client %d\n", usernames[index].sock_fd);
                }
            }
        }
    }

    // Should never get here.
    return 1;
}
