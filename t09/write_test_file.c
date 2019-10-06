#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

int main(int argc, char ** argv) {
  if (argc != 2) {
      fprintf(stderr, "Usage: write_test_file s filename\n");
      exit(1);
  }

  FILE *fp;
  // argv[1] contains the file name
  if ((fp = fopen(argv[1], "wb")) == NULL) { // create binary file to write into
    perror("fopen");
    exit(1);
  }

  for(int i = 0; i < 100; i++){
    // MAKE THESE WRITE RANDOM VALUES ---> IS THIS RIGHT CHECK
    int rand_num = random() % 100;
    if (fwrite(&rand_num, sizeof(int), 1, fp) == -1) {    // Read+Write for later ...
      perror("fwrite");
      exit(1);
    }
  }

  return 0;

  // should take a single argument representing a filename
  // and create a file with that name that contains 100 integers.



}
