#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char **argv) {

	int i;
	int n = 0;
	int num_kids;
	int status;

	if (argc != 2) {
		fprintf(stderr, "Usage: childcreates <numkids>\n");
		exit(1);
	}

	num_kids = strtol(argv[1], NULL, 10);

	for (i = 0; i < num_kids; i++) {
		if(n == 0){
			n = fork();
		}
		if (n < 0) {
			perror("fork");
			exit(1);
		}

 		printf("pid = %d, ppid = %d, i = %d\n", getpid(), getppid(), i);
		if(n > 0){
			// parent
			break; // wait for the kid
		}
	}

	wait(&status);

	return 0;
}
