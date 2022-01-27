#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void win(char *ai, char *human, int* ai_score, int* human_score) {
  int ;
  if (strcmp(ai, human) == 0) {
    printf("It's a tie\n");
  } else if (strcmp(ai, "rock") == 0) {
    if (strcmp(human, "paper") == 0) {
      printf("Paper covers rock\n");
      human_score++;
      printf("ai: %d, human: %d", ai_score, human_score);
    } else if (strcmp(human, "scissors") == 0) {
      printf("Rock bashes scissors\n");
      ai_score++;
      printf("ai: %d, human: %d", ai_score, human_score);
    } 
  } else if (strcmp(ai, "paper") == 0) {
    if (strcmp(human, "rock") == 0) {
      printf("Paper covers rock\n");
      ai_score++;
      printf("ai: %d, human: %d", ai_score, human_score);
    } else if (strcmp(human, "scissors") == 0) {
      printf("Scissors cut paper\n");
      human_score++;
      printf("ai: %d, human: %d", ai_score, human_score);
    }
  } else if (strcmp(ai, "scissors") == 0) {
    if (strcmp(human, "rock") == 0) {
      printf("Rock bashes scissors\n");
      human_score++;
      printf("ai: %d, human: %d", ai_score, human_score);
    } else if (strcmp(human, "paper") == 0) {
      printf("Scissors cut paper\n");
      ai_score++;
      printf("ai: %d, human: %d", ai_score, human_score);
    }
  }
}

int main() {
  int num_of_rounds;
  char *human_choice;
  char *ai_choice;
  int random_number;
  int ai_total_score;
  int human_total_score;

  srand(time(0));

  printf("Welcome to Rock, Paper, Scissors!\n");
  printf("How many rounds do you want to play? ");
  scanf("%d", &num_of_rounds);

  for (int i = 0; i < num_of_rounds; i++) {
    printf("Which do you choose? rock, paper, or scissors? ");
    scanf("%s", human_choice);
    
    random_number = rand() % 3;

    if (random_number == 0) {
      ai_choice = "rock";
    } else if (random_number == 1) {
      ai_choice = "paper";
    } else {
      ai_choice = "scissors";
    }

    printf("AI chose %s\n", ai_choice);

    if ((strcmp(human_choice, "rock") != 0) && (strcmp(human_choice, "paper") != 0) && (strcmp(human_choice, "scissors") != 0)) {
      printf("You entered an invalid choice: %s\n", human_choice);
    }
  
    win(ai_choice, human_choice, ai_total_score, human_total_score);
    printf("AI score: %d, Player score: %d\n\n", ai_total_score, human_total_score);
  }
  return 0;
}

