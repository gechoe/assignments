/**
 * password.c
 * Author: Grace Choe
 * Date: 1/25/2022
 * 
 * Description: 
 * This program asks the user for a word input. It then gives the user
 * a bad password where the word's 'e's are replaced with '3's and 'l's 
 * are replaced with '1's and 'a's are replaced with '@'s
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//main function
int main() {
  //Pointer variable declared to point to allocated heap space.
  char *word_array = NULL;

  //Call malloc to allocate the appropriate number of bytes for the array.
  word_array = malloc(sizeof(char) * 100);

  //Checks return value and handles error.
  if (word_array == NULL) {
    printf("Error: malloc has failed.");
    exit(1);
  }

  //Asks user for input.
  printf("Enter a word: ");
  //Gets user input and assigns to dynamically allocated word_array array.
  word_array = fgets(word_array, 100 * sizeof(char), stdin);

  //Replacing 'e's with '3's, 'l's with '1's, and 'a's with '@'s. 
  for (int i = 0; i < 100; i++) {
    if (word_array[i] == 'e') {
      word_array[i] = '3';
    } else if (word_array[i] == 'l') {
      word_array[i] = '1';
    } else if (word_array[i] == 'a') {
      word_array[i] = '@';
    }
  }
  
  //Prints out bad password.
  printf("Your bad password is: %s", word_array);
  
  //Frees memory and cleans up array to null.
  free(word_array);
  word_array = NULL;

  return 0;
}
