#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
  struct ppm_pixel **array;
  int width = 0;
  int height = 0;
  char period[2] = ".";
  unsigned char oldcolorvalue, newcolorvalue;
  char *token;

  if (argc < 2) {
    printf("Error: A filename must be entered.\n");
    exit(1);
  } else if (argc > 2) {
    printf("Error: Only one filename must be entered.\n");
  }
 
  array = read_ppm(argv[1], &width, &height);
  printf("Reading %s with width  %d and height %d\n", argv[1], width, height);

  token = strtok(argv[1], period);
  char name[] = "-glitch.ppm";
  strcat(token, name);
  
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      for (int m = 0; m < 3; m++) { 
        if (m == 0) {
          oldcolorvalue = array[i][j].red;
        } else if  (m ==1) {
          oldcolorvalue = array[i][j].green;
        } else {
          oldcolorvalue = array[i][j].blue;
        }

        newcolorvalue = oldcolorvalue << (rand() % 2);

        if (m == 0) {                                                            
          array[i][j].red = newcolorvalue;                                       
        } else if (m == 1) {                                                              
          array[i][j].green = newcolorvalue;                                     
        } else {                                                                 
          array[i][j].blue = newcolorvalue;                       
        }
      }
    }
  }

  write_ppm(token, array, width, height);
  printf("Writing file %s\n", token);

  for (int n = 0; n < height; n++) {
    free(array[n]);
    array[n] = NULL;
  }

  free(array);
  array = NULL;

  return 0;
}
