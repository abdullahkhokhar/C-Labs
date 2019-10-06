#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXLINE 256
#define MAX_PASSWORD 10

#define SUCCESS "Password verified\n"
#define INVALID "Invalid password\n"
#define NO_USER "No such user\n"

int main(void) {
  char user_id[MAXLINE];
  char password[MAXLINE];

  if(fgets(user_id, MAXLINE, stdin) == NULL) {    // gets the user_id given
      perror("fgets");
      exit(1);
  }
  if(fgets(password, MAXLINE, stdin) == NULL) {   // gets the password given
      perror("fgets");
      exit(1);
  }

  // in user_id and password we have the commmand line arguments stored
  // create a new process to run the validate program
  // send new process the user id and password
  // then print messege to stdout

  int fd[2], r;

  if ((pipe(fd)) == -1) {   // SET THE PIPE UP
      exit(1);
  }

  if((r = fork()) > 0){
    // parent process

    // Parent not reading from pipe
    close(fd[0]);

    // we want to write to the pipe from here
    write(fd[1], user_id, MAX_PASSWORD);
    write(fd[1], password, MAX_PASSWORD);

    close(fd[1]);  // done writing

    int status;
    if (wait(&status) != -1)  {       // WAIT FOR CHILD TO COMPELTE, GET EXIT STATUS
      if (WIFEXITED(status)) {
        // DO STUFF ACCORDING TO EXIT STATUS OF CHILD
                int out = WEXITSTATUS(status);
                if(out == 0){
                  printf("%s", SUCCESS);
                }else if(out == 2){
                  printf("%s", INVALID);
                }else{
                  printf("%s", NO_USER);
                }
              }
    }

  }
  else{
    // in the child process we will call exec for validate

    // Child not writing to pipe
    if ((close(fd[1])) == -1) {
        perror("close");
    }

    dup2(fd[0], fileno(stdin)); // READ FROM THE PIPE RATHER THAN STAD IN
    execl("./validate", NULL);
  }


  return 0;
}
