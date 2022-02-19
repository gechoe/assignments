/**
 * test_ppm.c
 * Author: Grace Choe
 * Date: 2/17/2022
 *
 * Description:
 * This program is a short test that that calls the read_ppm.c function and
 * prints the contents of feep-ascii.ppm (this file name is hard-coded into
 * this test).
 *
 * IMPORATANT NOTE!
 * The name of the "feep-ascii.ppm" PPM file is hard-coded in as the printed out
 * example on the Assignment 4, doesn't show the user inputting a filename, but
 * rather automatically using feep-ascii.ppm!
 */

#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

//main function
int main() {
  //Creates a struct 2D array called array_test.
  struct ppm_pixel **array_test;
  //char array string for filename.
  char string[20] = "feep-ascii.ppm";
  int width = 0;
  int height = 0;
  
  //Uses the function read_ppm() with the filename (string) input and width 
  //and height inputs.
  array_test = read_ppm(string, &width, &height);

  //Prints the filename and the width and height.
  printf("Testing file %s: %d %d\n", string, width, height);
  
  //Traverses through the 2D array and prints out all the RGB values.
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      printf("(%d,%d,%d) ", array_test[i][j].red, array_test[i][j].green,
        array_test[i][j].blue);
    }

    //Frees the arrays within the arrays in the 2D array.
    free(array_test[i]);
    array_test[i] = NULL;
    printf("\n");
  }
  
  //Frees the remaining arrays of the remnants of the 2D array.
  free(array_test);
  array_test = NULL;

  return 0;
}

