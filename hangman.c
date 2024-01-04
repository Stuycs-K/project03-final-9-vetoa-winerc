#include "hangman.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>


void err(int line) {
  printf("hangman.c line %d: %s\n", line, strerror(errno));
  exit(1);
}

char checkLetterGuess(char letter) {
    int r_file = open("word.txt", O_RDONLY | O_CREAT);
    if(r_file == -1) {
        err(17);
    }
    char* targetWord;
    read(r_file, )
    if(strstr())
}

char checkWordGuess(char* word) {

}