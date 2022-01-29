/**
 * rpc.c
 * Author: Grace Choe
 * Date: 1/26/2022
 * 
 * Description:
 * This program plays rock, paper, scissors with the user. They ask the
 * user for how many rounds they want to be played, then the user plays
 * against an AI and after every turn the points are calculated accordingly.
 * Once the round is over, the individual who won is stated.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

/**
 * win function: Calculates which individual won the round with the ai's and
 * human's rock paper, scissors choice as input and the ai's and human's score
 * as input. Then the points are updated accordingly.
 */
void win(char *ai, char *human, int* ai_score, int* human_score) {
  //Allows for points to be updated and brought back to main function.
  int ai_local = *ai_score;
  int human_local = *human_score;

  //Uses strcmp to compare who won and updates the points accordingly.
  if (strcmp(ai, human) == 0) {
    printf("It's a tie\n");
  } else if (strcmp(ai, "rock") == 0) {
    if (strcmp(human, "paper") == 0) {
      printf("Paper covers rock\n");
      human_local++;
    } else if (strcmp(human, "scissors") == 0) {
      printf("Rock bashes scissors\n");
      ai_local++;
    }
  } else if (strcmp(ai, "paper") == 0) {
    if (strcmp(human, "rock") == 0) {
      printf("Paper covers rock\n");
      ai_local++;
    } else if (strcmp(human, "scissors") == 0) {
      printf("Scissors cut paper\n");
      human_local++;
    }
  } else if (strcmp(ai, "scissors") == 0) {
    if (strcmp(human, "rock") == 0) {
      printf("Rock bashes scissors\n");
      human_local++;
    } else if (strcmp(human, "paper") == 0) {
      printf("Scissors cut paper\n");
      ai_local++;
    }
  }

  //Points able to be brought back to main function.
  *ai_score = ai_local;
  *human_score = human_local;
}

//main function
int main() {
  int num_of_rounds = 0;
  //Pointer variables declared to point to allocated heap space.
  char *human_choice = NULL;
  char *ai_choice = NULL;
  int random_number = 0;
  int ai_total_score = 0;
  int human_total_score = 0;

  //Call malloc to allocate the appropriate number of bytes for the arrays.
  human_choice = malloc(sizeof(char) * 50);
  ai_choice = malloc(sizeof(char) * 50);

  //Checks return value and handles error.
  if (human_choice == NULL) {
    printf("Error: malloc has failed.");
    exit(1);
  }

  //Checks return value and handles error.
  if (ai_choice == NULL) {
    printf("Error: malloc has failed.");
    exit(1);
  }

  //Seeds rand with a starting value when randomly generating numbers.
  srand(time(0));

  /**
   * Welcomes user to rock, paper, scissors, then ask user for input on how many
   * rounds to play and scans user input.
   */
  printf("Welcome to Rock, Paper, Scissors!\n");
  printf("How many rounds do you want to play? ");
  scanf("%d", &num_of_rounds);
  printf("\n");

  //for loop to play the amount of rounds user inputs.
  for (int i = 0; i < num_of_rounds; i++) {
    //Asks user for rock, paper, scissors input and scans input in.
    printf("Which do you choose? rock, paper, or scissors? ");
    scanf("%s", human_choice);
    
    //Lowercases all letters in user input.
    for (int i = 0; i < strlen(human_choice); ++i) {
      human_choice[i] = tolower((unsigned char)human_choice[i]);
    }

    //Random number generated from 0 to 2, so 0, 1, and 2.
    random_number = rand() % 3;

    //Each of the 3 random generated numbers is assigned a rock, paper, scissors value.
    if (random_number == 0) {
      strcpy(ai_choice, "rock");
    } else if (random_number == 1) {
      strcpy(ai_choice, "paper");
    } else {
      strcpy(ai_choice, "scissors");
    }

    printf("AI chose %s\n", ai_choice);

    //Checks if user input is valid.
    if ((strcmp(human_choice, "rock") != 0) && (strcmp(human_choice, "paper") != 0) && 
     (strcmp(human_choice, "scissors") != 0)) {
      printf("You entered an invalid choice: %s\n", human_choice);
    }

    //Performs win function to see who won the round, points updated.
    win(ai_choice, human_choice, &ai_total_score, &human_total_score);
    printf("AI score: %d, Player score: %d\n\n", ai_total_score, human_total_score);
  }

  //Prints out who won after the last round is played.
  if (ai_total_score > human_total_score) {
    printf("AI wins! Better luck next time!\n");
  } else if (ai_total_score < human_total_score) {
    printf("Player wins! Good job!\n");
  } else {
    printf("It's a tie! Try again to win!\n");
  }

  //Frees memory and cleans up array to null.
  free(human_choice);
  human_choice = NULL;
  
  //Frees memory and cleans up array to null.
  free(ai_choice);
  ai_choice = NULL;

  return 0;
}
