#include "hangman.h"


void err(int line) {
  printf("hangman.c line %d: %s\n", line, strerror(errno));
  exit(1);
}

char checkLetterGuess(char letter) {
    FILE *r_file;
    char word[21];//max word length of 20 characters
    char guessProgress[21]; //guess progress (asterisks represent unknowns)
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
    for(int i = 0; i < word[i] != '\0';i++ ) {
      if(word[i] == letter) {//compare target word letter by letter with current guess progress
        guessProgress[i] = letter; //replace asterisk with letter if guess matches
      } 
      else {
        guessProgress[i] = '*'; //keep asterisk an asterisk (need to include for first guess where guess progress is empty)
      }

    }
    fclose(r_file);
    guessResult(result);
    return result;
}

char checkWordGuess(char* target) {
  FILE *r_file;
  char word[21];//max word length of 20 characters
  int result = 0;
  r_file = fopen("word.txt", "r");
  if(r_file == NULL) {
      err(40);
  }

  if(fscanf(r_file, "%20s", word) != 1) err(36);//incorrectly formatted word txt file
  else {
    if(!strcasecmp(word, target))
      result++; //words don't match so result is false
  }
  guessResult(result);
  return result; 

  
}

void guessResult(int result) {
  if(result)
    printf("Correct!\n"); //printf for now
  else 
    printf("Guess is incorrect\n");
}