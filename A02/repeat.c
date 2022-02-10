/**
 * repeat.c
 * Author: Grace Choe
 * Date: 2/3/2022
 * 
 * Description:
 * This program asks the user for a string, s, and an integer, n, and then
 * creates a new string that repeats s n times. Example: The user inputs "ha"
 * and the number 3, the program outputs/returns "hahaha".
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//main function
int main() {
  int repeat = 0;
  //Will hold input by user. Can also be a dynamic. alloc. array.
  //Program assumes user enters words that are smaller than 32 characters.
  char word_array[32];
  //Pointer variable declared to point to allocated heap space.
  char *word_repeat_array = NULL;
  
  //Call malloc to allocate the appropriate number of bytes for the array.
  word_repeat_array = malloc(sizeof(char) * 2048);

  //Checks return value and handles error.
  if (word_repeat_array == NULL) {
    printf("Error: malloc has failed.");
    exit(1);
  }

  //Asks user for a word input.
  printf("Enter a word: ");
  //Gets user input and assigns to dynamically allocated word_array array.
  scanf("%s", word_array);

  //Asks user input for how many times to repeat word.
  printf("Enter a count: ");
  //Gets user input and assigns to repeat variable.
  scanf("%d", &repeat);

  //Fixed error Conditional jump or move depends on uninitialised value.
  //By initializing the word_repeat_array by strcpy.
  strcpy(word_repeat_array, word_array);

  //if statement to ensure that new string can be allocated within malloc.
  if ((strlen(word_array) * repeat) < (sizeof(char) * 2048)) {
    //If string can be allocated, for loop to repeat word n amount of times.
    for (int i = 0; i < repeat - 1; i++) {
      //strcat, apends user input word n amount of times in word_repeat_array.
      strcat(word_repeat_array, word_array);
    }
    //Prints the new string with repeated word.
    printf("%s\n", word_repeat_array);
  } else {
    //else, if string cannot be allocated, print:
    printf("Cannot allocate new string. Exiting...\n");
  }

  //Frees memory and cleans up array to null.
  free(word_repeat_array);
  word_repeat_array = NULL;

  return 0;
}
