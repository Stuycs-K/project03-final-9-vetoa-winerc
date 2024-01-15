#include "hangman.h"
#include "hangman_server.h"
#include "networking.h"
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

void err(int line) {
  printf("hangman.c line %d: %s\n", line, strerror(errno));
  exit(1);
}

struct game_info* endGame(struct game_info* game) {
  char message[MESSAGE_SIZE];
  if (game->num_guesses == 0) {
    sprintf(message, "No more guesses. You lost! The word was %s.\n", game->real_word);
  }
  else {
    sprintf(message, "You won with %d guesses remaining! The word was %s.\n", game->num_guesses, game->real_word);
  }

  message_blast(game, message, -1);

  free(game->guessing_order);
  game->guessing_order = NULL;
  game->guesser_index = 0;
  game->num_guesses = 0;
  game->guesser = 0;
  game->real_word = NULL;
  free(game->current_word);
  game->current_word = NULL;
  free(game->failed_guesses);
  game->failed_guesses = NULL;

  return game;
}

struct game_info* advanceGame(struct game_info* game) {
  if (game->guessing_order != NULL) {
    game->guesser_index++;
    // max index is num clients - 1
    if (game->guesser_index > game->num_clients - 1 || (game->guesser_index > game->num_clients - 2 && game->gamemode == USER_CHOOSING)) {
      game->guesser_index = 0;
    }
    game->guesser = game->guessing_order[game->guesser_index];
    if (game->num_guesses > 0) {
      write(game->client_sockets[game->guessing_order[game->guesser_index]], "guess", 6);
    }
    usleep(50);
  }

  return game;
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

  // end game stuff
  if (strcmp(game->current_word, game->real_word) == 0) {
    game = endGame(game);
  }

  if (success == 0) {
    int offset = letter - 'a';
    game->failed_guesses[offset] = letter;
    game->num_guesses--;
    // end game stuff
    if (game->num_guesses == 0) {
      game = endGame(game);
    }
  }

  game = advanceGame(game);

  return game;
}

struct game_info* checkWordGuess(struct game_info* game, char* target) {
  char success = 0;
  for (int i = 0; i < strlen(target); i++) {
    target[i] = tolower(target[i]);
  }

  // end game stuff
  if (strcmp(game->real_word, target) == 0) {
    game = endGame(game);
  }

  if (success == 0) {
    game->num_guesses--;
    // end game stuff
    if (game->num_guesses == 0) {
      game = endGame(game);
    }
  }

  game = advanceGame(game);

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
    printf("asking %s for the starting word\n", game->usernames[game->chooser]);
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
    // printf("chooser: %d\n", game->chooser);
    game->guessing_order = malloc(sizeof(int) * (game->num_clients - 1));
    for (int i = 0; i < game->num_clients; i++) {
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
        // printf("assigned %s\n", game->usernames[i]);
    }
    }
  }

  game->guesser = game->guessing_order[0];
  game->guesser_index = 0;
  write(game->client_sockets[game->guessing_order[game->guesser_index]], "guess", 6);
  // if hasn't set the num guesses, change it to 5 
  if (game->num_guesses <= 0) {
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