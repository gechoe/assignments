/**
 * read_ppm.c
 * Author: Grace Choe
 * Date: 4/22/2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

struct ppm_pixel** read_ppm(const char* filename, int* w, int* h) {
  // your code here
  FILE *file;
  struct ppm_pixel **array;
  char *s = NULL;
  int width = *w;
  int height = *h;
  char *whitespace = " \t\f\r\v\n";
  char *token1, *token2;
  
  //s used to move the pointer and read in everything.
  //(Used to find the location of the width, hieght, and color information.
  s = malloc(sizeof(char) * 100);
  
  if (s == NULL) {
    printf("Error: Malloc failed!\n");
    exit(1);
  }
  
  //Opens the file in read binary mode.
  file = fopen(filename, "rb");
  
  if (file == NULL) {
    printf("Error: The file cannot be read!");
  }
  
  fgets(s, 100, file);
  fgets(s, 100, file);
 
  //while loop, goes through file reading through comments until the color info.
  //portion is found.
  while (s[0] == '#') {
    fgets(s, 100, file);
  }

  //width portion tokened out, to get the width from the file.
  token1 = strtok(s, whitespace);
  width = atoi(token1);
  
  //height portion tokened out, to get the height from the file.
  token2 = strtok(NULL, whitespace);
  height = atoi(token2);
  
  //Creates the 2D array using the height and width information.
  array = malloc(sizeof(struct ppm_pixel *) * height);
  
  for (int i = 0; i < height; i++) {
    array[i] = malloc(sizeof(struct ppm_pixel) * width);
  }
  
  if (array == NULL) {
    return NULL;
  }
  
  fgets(s, 100, file);

  char *string;

  //string used to copy all the pixel data into an array as one large block later.
  string = malloc(sizeof(char) * width * height * 3);

  if (string == NULL) {
    printf("Error: malloc failed!");
    exit(1);
  }

  fread(string, sizeof(char), width * height * 3, file);
  int m = 0;
  
  //Used to assign each pixel data to its place within the array.
  for (int j = 0; j < height; j++) {
    for (int k = 0; k < width; k++) {
      //fread(string, sizeof(char), width * height * 3, file);
      array[j][k].red = string[m];
      array[j][k].green = string[m + 1];
      array[j][k].blue = string[m + 2];
      m += 3;
    }
  }
  
  //Frees
  free(s);
  s = NULL;
  
  free(string);
  string = NULL;

  //Brings back updated info. to the main function.
  *w = width;
  *h = height;

  //Closes file.
  fclose(file);
  
  return array;
}


void write_ppm(const char* filename, struct ppm_pixel** pxs, int w, int h) {
  // your code here
  FILE *file_w;
  char type[3] = "P6";
  char top[4] = "255";
  char width[500], height[500];
  char enter = '\n';

  //Opens file in write mode.
  file_w = fopen(filename, "wb");
  
  if (file_w == NULL) {
    printf("Error: unable to open file %s\n", filename);
    exit(1);
  }
  
  //Converts width and height into string, to prepare for the headers to be put
  //in the file.
  sprintf(width, "%d", w);
  sprintf(height, "%d", h);

  //Brings strings together according to the file header format.
  strcat(width, " ");
  strcat(width, height);
  
  //for loop, puts the "P6" header portion into the file without the null
  //terminator.
  for (int i = 0; i < strlen(type); i++) {
    if (type[i] != '\0') {
      fwrite(&type[i], sizeof(type[i]), 1, file_w);
    }
  }
  
  //Writes in new line.
  fwrite(&enter, sizeof(enter), 1, file_w);
  
  //for loop, puts the width and height header portion into the file without the
  //null terminator.
  for (int i = 0; i < strlen(width); i++) {
    if (width[i] != '\0') {
      fwrite(&width[i], sizeof(width[i]), 1, file_w);
    }
  }
  
  //Writes in new line.
  fwrite(&enter, sizeof(enter), 1, file_w);
 
  //for loop, puts the highest number/limit pixel into the file without the null
  //terminator.
  for (int i = 0; i < strlen(top); i++) {
    if (top[i] != '\0') {
      fwrite(&top[i], sizeof(top[i]), 1, file_w);
    }
  }
  
  //Writes in new line.
  fwrite(&enter, sizeof(enter), 1, file_w);
  
  //for loop, puts/writes in the file the pixel color information.
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      fwrite(&pxs[i][j], sizeof(pxs[i][j]), 1, file_w);
    }
  }
  
  //Closes file.
  fclose(file_w);
}

