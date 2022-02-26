#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
  //Creates a struct 2D array called array_test.
  struct ppm_pixel **array_test;
  //char array string for filename.
  char string[20] = "feep-raw.ppm";
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
