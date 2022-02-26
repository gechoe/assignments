#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
  //FILE *filew;
  struct ppm_pixel **array;
  int width = 0;
  int height = 0;
  //char *string = NULL;
  char period[2] = ".";
 
  char *token;
  if (argc < 2) {
    printf("Error: A filename must be entered.\n");
    exit(1);
  } else if (argc > 2) {
    printf("Error: Only one filename must be entered.\n");
  }
 

 /*
  int size = strlen(argv[1]);

  string = malloc(sizeof(char) * (size + 1));

  if (string == NULL) {
    printf("Error: malloc failed!\n");
    exit(1);
  }*/

  //string = argv[1];

  array = read_ppm(argv[1], &width, &height);
  printf("Reading %s with width  %d and height %d\n", argv[1], width, height);

  token = strtok(argv[1], period);
  
  //printf("%s", token);
  char name[] = "-glitch.ppm";

  strcat(token, name);
  
  write_ppm(token, array, width, height);
  printf("Writing file %s\n", token);

/*  filew = fopen(string, "wb");

  if (filew == NULL) {
    printf("Cannot open file %s!\n", string);
    exit(1);
  }

  fwrite();
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      printf("(%d,%d,%d) ", array_test[i][j].red, array_test[i][j].green,
        array_test[i][j].blue);
    }

    free(array_test[i]);
    array_test[i] = NULL;
    printf("\n");
  }
 
  free(array_test);
  array_test = NULL;

  */
  for (int i = 0; i < height; i++) {
    //for (int j = 0; j < width; j++) {
      free(array[i]);
      array[i] = NULL;
    //}
  }

  free(array);
  array = NULL;

  return 0;
}
