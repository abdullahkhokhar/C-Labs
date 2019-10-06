#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    Complete this program by writing the function strip_q_marks that takes
    a single string and returns an integer.

    The function should modify the string to remove any trailing question marks
    and return the number of question marks that were removed.

    Examples
    original sentence       modified sentence       return value
    =============================================================
    "Hello? World???"       "Hello? World"          3
    "What..?"               "What.."                1
    "Apples?..?"            "Apples?.."             1
    "Coffee"                "Coffee"                0
*/

// Write the function strip_q_marks here

int strip_q_marks(char* s){
  // so we need to remove all trailling question marks from the friend
  // until the first NON-question mark character
  int string_length = strlen(s);
  int indx = string_length - 1; // start from the back
  char curr = s[indx];

  while(curr == '?' && indx != -1){
    indx = indx - 1;
    curr = s[indx];
  }
  s[indx + 1] = '\0';
  return (string_length-1) - (indx);
}

int main(int argc, char **argv) {
    // Do not change this main function.
    if(argc != 2) {
        fprintf(stderr, "Usage: strip message\n");
        exit(1);
    }
    int result = strip_q_marks(argv[1]);
    printf("%s %d", argv[1], result);
    return 0;
}
