/**
 * ascii_image.c
 * Author: Grace Choe
 * Date: 2/18/2022
 * 
 * Description:
 * This program takes a PPM image as a command line argument and displays it as 
 * ASCII Art.
 */

#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

//main function
int main(int argc, char** argv) {
  // todo
  //Creates a struct 2D array called image_array.
  struct ppm_pixel **image_array;
  int width = 0;
  int height = 0;
  int image_val = 0;

  //Handles errors where: 1)There is less input than required in the commandline
  //or 2)There is more input than required on the commandline.
  if (argc < 2) {
    printf("Error: A filename must be entered.\n");
    exit(1);
  } else if (argc > 2) {
    printf("Error: Only one filename must be entered.\n");
    exit(1);
  }

  //Uses the function read_ppm().
  image_array = read_ppm(argv[1], &width, &height);
 
  //Prints the filename and the width and height. 
  printf("Reading %s with width %d and height %d\n", argv[1], width, height);
  
  /* Traverses through the 2D array and for each RGB pixel, the average
  intensity is computed as follows: I = 1/3 * (R + G + B) */
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      image_val = ((image_array[i][j].red - '\0') + 
        (image_array[i][j].green - '\0') + (image_array[i][j].blue - '\0')) / 3;
      
      //If statement: Assigns an ASCII character based on intensity.
      if (image_val >= 0 && image_val <= 25) {
        printf("@");
      } else if (image_val >= 26 && image_val <= 50) {
        printf("#");
      } else if (image_val >= 51 && image_val <= 75) {
        printf("%%"); 
      } else if (image_val >= 76 && image_val <= 100) {
        printf("*"); 
      } else if (image_val >= 101 && image_val <= 125) {
        printf("o"); 
      } else if (image_val >= 126 && image_val <= 150) {
        printf(";"); 
      } else if (image_val >= 151 && image_val <= 175) {
        printf(":"); 
      } else if (image_val >= 176 && image_val <= 200) {
        printf(","); 
      } else if (image_val >= 201 && image_val <= 225) {
        printf("."); 
      } else if (image_val >= 226 && image_val <= 255) {
        printf(" "); 
      }
      //Resets the image_val to 0.
      image_val = 0;
    }
    printf("\n");
  }
  
  //Frees the arrays within the arrays in the 2D array.
  for (int k = 0; k < height; k++) {
    for (int m = 0; m < height; m++) {
      free(image_array[k]);
      image_array[k] = NULL;
    }
  }

  //Frees the arrays in the 2D array.
  free(image_array);
  image_array = NULL;

  return 0;
}
