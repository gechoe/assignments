/**
 * glitch.c
 * Author: Grace Choe
 * Date: 2/25/2022
 *
 * Description:
 * This program is reads in a PPM file and "glitches" it. This program then saves
 * the modified PPM in a new file with the suffix "-glitch".
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

//main function
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
    exit(1);
  }
  
  //Uses the read_ppm function to read the file and make the array.
  array = read_ppm(argv[1], &width, &height);
  printf("Reading %s with width %d and height %d\n", argv[1], width, height); 
  
  //Creates token to make new file name.
  token = strtok(argv[1], period);
  char name[] = "-glitch.ppm";
  
  //Creates new file name.
  strcat(token, name);
  
  //Edits the array to "glitch" the pixel colors.
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
  
  //Uses the write_ppm function to write a new file with the "glitches".
  write_ppm(token, array, width, height);
  printf("Writing file %s\n", token);
  
  //Frees the arrays within the array (2D array)
  for (int k = 0; k < height; k++) {
    free(array[k]);
    array[k] = NULL;
  }
  
  //Frees the final outer array.
  free(array);
  array = NULL;
  
  return 0;
}
