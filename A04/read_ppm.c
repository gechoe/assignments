/**
 * read_ppm.c
 * Author: Grace Choe
 * Date: 2/17/2022
 *
 * Description:
 * This program reads a PPM file stored in ASCII format. It takes a filename as
 * input and returns a 2D array of struct pixel.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

// TODO: Implement this function
// Feel free to change the function signature if you prefer to implement an 
// array of arrays
struct ppm_pixel** read_ppm(const char* filename, int* w, int* h) {
  //FILE * variable declared.
  FILE *file;
  //A 2D array variable array is declared as 'struct ppm_pixel **'.
  struct ppm_pixel **array;
  //Pointer variable declared to point to allocated heap spacce, for reading
  //each line as a string.
  char *s = NULL;
  //Allows for points to be updated and brought back to main function.
  int width = *w;
  int height = *h;
  //char that takes into account all whitespace types.
  char *whitespace = " \t\f\r\v\n";
  //tokens for splitting the string that has the width and height of the file.
  char *token1, *token2;
  unsigned char r, g, b;

  //Call malloc to allocate the appropriate number of bytes for the array.
  s = malloc(sizeof(char) * 100);

  //Checks return value and handles error.
  if (s == NULL) {
    printf("Error: Malloc failed!\n");
    exit(1);
  }

  //Opens the file in read mode.
  file = fopen(filename, "r");

  //Checks return value and handles error.
  //Returns null if the filename is invalid.
  if (file == NULL) {
    return NULL;
  }

  //Reads in first line of the PPM file, involving ASCII types: "P3"
  fgets(s, 100, file);
  //Read in next line of the file.
  fgets(s, 100, file);
  
  //Checks if this next line is a comment by seeing if the first character of 
  //the string is a '#'.
  while (s[0] == '#') {
    //If it is a hashtag then it keeps going/reading to the next line until it's
    //not a comment.
    fgets(s, 100, file);
  }

  /*Escaping the loop means the next line has the width and height values.
    strtok is used to divide the string into tokens. token1's strtok gets the
    first token, the string before the next whitespace. */  
  token1 = strtok(s, whitespace);
  //token1/width is converted to int.
  width = atoi(token1);
  //The next token is gotten.
  token2 = strtok(NULL, whitespace);
  //token2/height is converted to int.
  height = atoi(token2);

  //Allocates an array of height pointers to struct ppm_pixel
  array = malloc(sizeof(struct ppm_pixel *) * height);
  
  //For loop, for each row, space malloc'd for its column elements and 
  //added to the array of arrays.
  for (int i = 0; i < height; i++) {
    array[i] = malloc(sizeof(struct ppm_pixel) * width);
  }
  
  //Returns NULL if memory cannot be allocated for the image data.
  if (array == NULL) {
    return NULL;
  }
  
  //Gets/reads next line, which holds the maximum color value as an ASCII
  //decimal integer.
  fgets(s, 100, file);

  //Traverses through the 2D array and sets the struct ppm_pixel properties
  //accordingly.
  for (int j = 0; j < height; j++) {
    for (int k = 0; k < width; k++) {
      //fscanf used to read unsigned char color values.
      fscanf(file, " %hhu %hhu %hhu", &r, &g, &b);
      array[j][k].red = r;
      array[j][k].green = g;
      array[j][k].blue = b;
    }
  }

  //Frees the string reaed from file by fgets.
  free(s);
  s = NULL;
  
  //Points able to be brought back to the main function.
  *w = width;
  *h = height;
  
  //Closes the file. 
  fclose(file);

  //Returns a pointer to the array created.
  return array;
}
