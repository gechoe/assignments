#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  //Pointer variable declared to point to allocated heap space.
  char *word_array;

  //Call malloc to allocate the appropriate number of bytes for the array.
  word_array = malloc(sizeof(char) * 50);

  //
  if (word_array == NULL) {
    printf("Error: malloc has failed.");
    exit(1);
  }

  printf("Enter a word: ");
  scanf("%s", word_array);

  for (int i = 0; i < 50; i++) {
    if (word_array[i] == 'e') {
      word_array[i] = '3';
    } else if (word_array[i] == 'l') {
      word_array[i] = '1';
    } else if (word_array[i] == 'a') {
      word_array[i] = '@';
    }
    
    word_array[49] = '\0'; //nullifyyYyyYYy fix this comment later(?)
  }
  
  printf("Your bad password is: %s\n", word_array);
  return 0;
}
