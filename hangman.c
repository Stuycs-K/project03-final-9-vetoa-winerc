#include "hangman.h"


void err(int line) {
  printf("hangman.c line %d: %s\n", line, strerror(errno));
  exit(1);
}

char checkLetterGuess(char letter) {
    FILE *r_file;
    char word[21];//max word length of 20 characters
    int result = 0;
    r_file = fopen("word.txt", "r");
    if(r_file == NULL) {
        err(15);
    }

    if(fscanf(r_file, "%20s", word) != 1) err(18);//incorrectly formatted word txt file
    else {
      if(strchr(word, letter) != NULL)
        result++; //found the letter so return value is true
    }
    fclose(r_file);
    return result;
}

char checkWordGuess(char* word) {

}