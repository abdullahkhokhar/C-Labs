#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char **argv) {

	int i;
	int n = 1;
	int num_kids;
	int status;

	if (argc != 2) {
		fprintf(stderr, "Usage: parentcreates <numkids>\n");
		exit(1);
	}

	num_kids = strtol(argv[1], NULL, 10);

	for (i = 0; i < num_kids; i++) {
		if(n==0){
			exit(0);
		}
		if(n>0){
			// only fork if you are a parent
			n = fork();
		}
		if (n < 0) {
			perror("fork");
			exit(1);
		}
 		printf("pid = %d, ppid = %d, i = %d\n", getpid(), getppid(), i);
	}

	for (i = 0; i < num_kids; i++) {
		// we want to wait for each child the parent creates 
		wait(&status);
	}


	return 0;
}
