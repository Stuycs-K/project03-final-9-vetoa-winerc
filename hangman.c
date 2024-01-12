#include "hangman.h"
#include "hangman_server.h"
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

void err(int line) {
  printf("hangman.c line %d: %s\n", line, strerror(errno));
  exit(1);
}

struct game_info* checkLetterGuess(struct game_info* game, char letter) {
  char success = 0; // not a success
  letter = tolower(letter);
  for (int i = 0; i < strlen(game->real_word); i++) {
    if (game->real_word[i] == letter) {
      game->current_word[i] = letter;
      success = 1;
    }
  }

  if (success == 0) {
    int offset = letter - 'a';
    game->failed_guesses[offset] = letter;
    game->num_guesses--;
  }
  return game;
}

struct game_info* checkWordGuess(struct game_info* game, char* target) {
  char success = 0;
  for (int i = 0; i < strlen(target); i++) {
    target[i] = tolower(target[i]);
  }

  if (strcmp(game->real_word, target) == 0) {
    game->current_word = target;
    game->num_guesses = 0;
    success = 1;
  }
  if (success == 0) {
    game->num_guesses--;
  }

  return game;
}

void guessResult(int result) {
  if(result)
    printf("Correct!\n"); //printf for now
  else 
    printf("Guess is incorrect\n");
}

struct game_info* setStartingWord(struct game_info* game) {
  if (game->gamemode == COMPUTER_CHOOSING) {
    game->real_word = "hangman"; // hardcoded, will change later
  }
  else if (game->gamemode == USER_CHOOSING) {
    user_start_word(game);
  }
  // set current word based on real word
  game->current_word = malloc(strlen(game->real_word));
  for (int i = 0; i < strlen(game->real_word); i++) {
    game->current_word[i] = '*';
  }
  game->current_word[strlen(game->real_word)] = 0;
  return game;
}

struct game_info* startGame(struct game_info* game) {
  // guessing order
  srand(time(NULL));
  if (game->gamemode == COMPUTER_CHOOSING) {
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
  }
  else {
    game->guessing_order = malloc(sizeof(int) * (game->num_clients - 1));
    for (int i = 0; i < game->num_clients - 1; i++) {
      game->guessing_order[i] = -1;
    }
    for (int i = 0; i < game->num_clients; i++) {
      // if the user isn't the chooser
      if (game->chooser != i) {
        // finds a random index array that hasn't been assigned
        int j = rand() % game->num_clients;
        while (game->guessing_order[j] != -1) {
          j = rand() % game->num_clients;
        }
        game->guessing_order[j] = i;
    }
    }
  }

  game->guesser = game->guessing_order[0];
  game->guesser_index = 0;
  // if hasn't set the num guesses, change it to 5 
  if (game->num_guesses == 0) {
    game->num_guesses = 5;
  }

  game = setStartingWord(game);
  game->failed_guesses = malloc(27);
  game->failed_guesses[26] = 0;
  for (int i = 0; i < 26; i++) {
    game->failed_guesses[i] = '*';
  }
  
  return game;
}

struct game_info* advanceGame(struct game_info* game) {
  game->guesser_index++;
  game->guesser = game->guessing_order[game->guesser_index];
  write(game->client_sockets[game->guessing_order[game->guesser_index]], "guess", 6);

  return game;
}