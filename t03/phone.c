#include <stdio.h>

int main() {

    char line[10];                // is this size for a line appropriate
    int given_integer = 0;

    while(scanf("%s %d", line, &(given_integer)) != EOF){
      // ASSUMING SPACE BETWEEN 10 CHARACTERS AND INTEGER
      // The while loop reads the entire line for us
      // Depending on this integer then we print what we need to
      if(given_integer == 0){
        // print the entire string
        printf("%s\n", line);
      }
      else if((given_integer >=1) && (given_integer <=9)){
        //print whatever is at the int given
        printf("%c\n", line[given_integer]);
      }
  }
    return 0;
}
