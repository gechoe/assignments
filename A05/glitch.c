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
 
  //string = argv[1];

  array = read_ppm(argv[1], &width, &height);
  printf("Reading %s with width  %d and height %d\n", argv[1], width, height);

  //printf("%d, %d", width, height);
  token = strtok(argv[1], period);
  
  //printf("%s", token);
  char name[] = "-glitch.ppm";

  strcat(token, name);
  
  write_ppm(token, array, width, height);
  printf("Writing file %s\n", token);

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
