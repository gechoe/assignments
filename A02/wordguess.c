/**
 * wordguess.c
 * Author: Grace Choe
 * Date: 2/3/2022
 *
 * Description:
 * Thiss program chooses a random word from a list and then asks the player to
 * guess the letters in the word to get the word.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//main function
int main() {
  //Declares a FILE * variable.
  FILE *infile;
  int total_num_words = 0;
  int random_number = 0;
  //Pointer variable declared to point to allocated heap space.
  /**
   * Since all words are lowercase and no greater than 32 characters long, could
   * have made char word_array[32]; but wanted to practice more using dynamic.
   * alloc. arrays.
   */
  char *word_array = NULL;
  char *guess_array = NULL;
  int turn = 1;
  char character_guess = 'z';
  char contains[2] = "_\0";

  //Call malloc to allocate the appropriate number of bytes for the array.
  word_array = malloc(sizeof(char) * 32);
  guess_array = malloc(sizeof(char) * 32);

  //Checks return value and handles error.
  if (word_array == NULL) {
    printf("Error: malloc has failed."); 
    exit(1);
  }

  //Checks return value and handles error.
  if (guess_array == NULL) {
    printf("Error: malloc has failed.");
  }

  //Open the file (fopen), relative path name of file, read mode.
  infile = fopen("words.txt", "r");
  //Checks return value and handles error.
  if (infile == NULL) {
    printf("Error: unable to open file %s\n", "words.txt");
    exit(1);
  }

  //Reads in value from the first line, which contains the total num of words.
  fscanf(infile, "%d", &total_num_words);

  //Seeds rand with a starting value when randomly generating numbers.
  srand(time(0));
  //Random number generated from 0 to the amount of words in the file.
  random_number = rand() % total_num_words;

  /**
   * for loop uses fgets to read in each line of text up until the randomized
   * num to get a randomized word.
   */
  for (int i = 0; i < random_number; i ++) {
    word_array = fgets(word_array, 32 * sizeof(char), infile);
  }

  //Closes the file.
  fclose(infile);

  //Copies the contents of word_array into guess_array.
  strcpy(guess_array, word_array);

  printf("Welcome to Word Guess!\n");

  //for loop replaces every copied letter within guess_array with underscore.
  for (int i  = 0; i < (strlen(guess_array) - 1); i++) {
    if (guess_array[i] != '\0') {
      guess_array[i] = '_';
    }
  }

  //while loop keeps going until all letters in word are found to get the word.
  while (1) {
    //...... added as a stylistic choice
    printf("\n..........");
    printf("\nTurn: %d\n\n", turn);

    //Prints out the guess_array to see how many letters are found.
    for (int j  = 0; j < (strlen(guess_array) - 1); j++) {
      printf("%c ", guess_array[j]);
    }

    //Asks for user character input and scans it in.
    printf("\nGuess a character: ");
    scanf("%s", &character_guess);

    //for loop replaces underscore with every correct character guess.
    for (int k = 0; k < (strlen(word_array)); k++) {
      if (word_array[k] == character_guess) {
        guess_array[k] = character_guess;
      }
    }

    //Searches within string guess_array if there is character_guess.
    char *word_check = strstr(guess_array, &character_guess);

    //If character guess not present print not found.
    if (!word_check) {
      printf("Sorry, %c not found!\n", character_guess);
    }

    //Checks if there is an underscore left in guess_array.
    char *done_check = strstr(guess_array, contains);

    //if no underscores left, print out full word and in how many turns user won.
    if (!done_check) {
      printf("\n");

      for (int m  = 0; m < (strlen(guess_array) - 1); m++) {
        printf("%c ", guess_array[m]);
      }

      printf("\nYou won in %d turns!\n", turn);
      //Breaks out of while loop.
      break;
    }

    //Increment turn.
    turn++;
  }

  //Frees memory and cleans up array to null.
  free(word_array);
  word_array = NULL;

  //Frees memory and cleans up array to null.
  free(guess_array);
  guess_array = NULL;

  return 0;
}
