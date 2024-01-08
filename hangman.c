#include "hangman.h"
#include "hangman_server.h"
#include <stdlib.h>
#include <time.h>


void err(int line) {
  printf("hangman.c line %d: %s\n", line, strerror(errno));
  exit(1);
}

char checkLetterGuess(char letter) {
    FILE *file;
    char word[21];//max word length of 20 characters
    char guessProgress[21]; //guess progress (asterisks represent unknowns)
    int result = 0;
    int guesses = 0;

    file = fopen("word.txt", "r+"); //use r+ to read and write
    if(file == NULL) err(16);
    
    if(fscanf(file, "%20s", word) != 1) err(20);//incorrectly formatted word txt file

    if(fscanf(file, "%20s", guessProgress) != 1) { //fill in guess progress w/ asterisks (handling for first guess)
      for(int i = 0; word[i] != '\0'; i++) {
          guessProgress[i] = '*';
      }
      guessProgress[strlen(word)] = '\0';
    }

    if(fscanf(file, "%d", &guesses) != 1) {
      guesses = 0;
    }
    guesses++;

    for(int i = 0; word[i] != '\0'; i++) {
      if(word[i] == letter) {
        guessProgress[i] = letter;
        result++; //letter found
      }
    }

    rewind(file); // go back to the start of the file so you can overwrite guessprogress.
    fprintf(file,"%s\n%s\n%d\n",word, guessProgress, guesses);
    fclose(file);
    // guessResult(result);
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

struct game_info* startGame(struct game_info* game) {
  // guessing order
  srand(time(NULL));
  game->guessing_order = malloc(sizeof(int) * game->num_clients);
  for (int i = 0; i < game->num_clients; i++) {
    game->guessing_order[i] = -1;
  }
  for (int i = 0; i < game->num_clients; i++) {
    // finds a random index array that hasn't been assigned
    int j = rand() % game->num_clients;
    while (game->guessing_order[j] != -1) {
      j = rand() % game->num_clients;
    }
    game->guessing_order[j] = i;
  }

  game->guesser = game->guessing_order[0];
  game->guesser_index = 0;
  return game;
}

struct game_info* advanceGame(struct game_info* game) {
  game->guesser_index++;
  game->guesser = game->guessing_order[game->guesser_index];

  return game;
}
