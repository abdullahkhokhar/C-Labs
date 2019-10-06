#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <signal.h>
#include <sys/time.h>


// Message to print in the signal handling function.
#define MESSAGE "%ld reads were done in %ld seconds.\n"


/* Global variables to store number of read operations and seconds elapsed.
 * These have to be global so that signal handler to be written will have
 * access.
 */
long num_reads = 0, seconds;

void handle(int signum){
	if(signum == SIGPROF){
		printf(MESSAGE, num_reads, seconds);
		exit(0); // WHAT DO WE NEED TO EXIT WIT
	}
}

int main(int argc, char ** argv) {
    // DEFINE A NEW HANDLER
    struct sigaction new_action;
    new_action.sa_handler = handle;
    new_action.sa_flags = 0;
    if(sigaction(SIGPROF, &new_action, NULL) == -1){
			exit(1); // error has occured
		}

    if (argc != 3) {
        fprintf(stderr, "Usage: time_reads s filename\n");
        exit(1);
    }
    seconds = strtol(argv[1], NULL, 10); // argument s representing a number of seconds

		struct itimerval time_val;
		time_val.it_value.tv_sec = seconds;
		time_val.it_value.tv_usec = 0;
		time_val.it_interval.tv_usec = 0;
		time_val.it_interval.tv_sec = 0;

		setitimer(ITIMER_PROF, &time_val, NULL);

    FILE *fp;                            // a binary file f that contains 100 integers
    if ((fp = fopen(argv[2], "r+")) == NULL) {    // Read+Write for later ...
      perror("fopen");
      exit(1);
    }

    /* In an infinite loop, read an int from a random location in the file
     * and print it to stderr.
     */
    for (;;) {
      int read_int;
      // program should seek to a random location in the file
      int rand_seek = (random() % 100)*4;
      fseek(fp, rand_seek, SEEK_SET);
      fread(&read_int, sizeof(int), 1, fp);
			num_reads +=1;
      // fprintf("%d\n", read_int); // confirmed from Lucas we dont need this 
    }

    return 1;  //something is wrong if we ever get here!
}
