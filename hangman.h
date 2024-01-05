#include "hangman.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

void err(int line);
char checkLetterGuess(char letter);
char checkWordGuess();