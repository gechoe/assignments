#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

// TODO: Implement this function
// Feel free to change the function signature if you prefer to implement an 
// array of arrays
struct ppm_pixel** read_ppm(const char* filename, int* w, int* h) {
  FILE *file;
  struct ppm_pixel **array;
  char *s = NULL;
  int width = *w;
  int height = *h;
  char *whitespace = " \t\f\r\v\n";
  char *token1, *token2;

  s = malloc(sizeof(char) * 100);

  if (s == NULL) {
    printf("Error: Malloc failed!\n");
    exit(1);
  }

  file = fopen(filename, "rb");

  if (file == NULL) {
    return NULL;
  }

  fgets(s, 100, file);
  fgets(s, 100, file);
  
  while (s[0] == '#') {
    fgets(s, 100, file);
  }
  
  token1 = strtok(s, whitespace);
  width = atoi(token1);
  
  token2 = strtok(NULL, whitespace);
  height = atoi(token2);

  array = malloc(sizeof(struct ppm_pixel *) * height);
  
  for (int i = 0; i < height; i++) {
    array[i] = malloc(sizeof(struct ppm_pixel) * width);
  }
  
  if (array == NULL) {
    return NULL;
  }

  fgets(s, 100, file);

  char *string;
  string = malloc(sizeof(char) * width * height * 3);

  if (string == NULL) {
    printf("Error: malloc failed!");
    exit(1);
  }

  fread(string, sizeof(char), width * height * 3, file);
  int m = 0;

  for (int j = 0; j < height; j++) {
    for (int k = 0; k < width; k++) {
      fread(string, sizeof(char), width * height * 3, file);
      array[j][k].red = string[m];
      array[j][k].green = string[m + 1];
      array[j][k].blue = string[m + 2];
      m += 3;
    }
  }

  free(s);
  s = NULL;

  free(string);
  string = NULL;
  
  *w = width;
  *h = height;
  
  fclose(file);

  return array;
}

// TODO: Implement this function
// Feel free to change the function signature if you prefer to implement an 
// array of arrays
extern void write_ppm(const char* filename, struct ppm_pixel** pxs, int w, int h) {
  FILE *file_w;
  char type[3] = "P6";
  char top[4] = "255";
  char width[500], height[500];
  char enter = '\n';
  
  file_w = fopen(filename, "wb");

  if (file_w == NULL) {
    printf("Error: unable to open file %s\n", filename);
    exit(1);
  }

  sprintf(width, "%d", w);
  sprintf(height, "%d", h);

  strcat(width, " ");
  strcat(width, height);

  for (int i = 0; i < strlen(type); i++) {
    if (type[i] != '\0') {
      fwrite(&type[i], sizeof(type[i]), 1, file_w);
    }
  }

  fwrite(&enter, sizeof(enter), 1, file_w);

  for (int i = 0; i < strlen(width); i++) {                                       
    if (width[i] != '\0') {                                                       
      fwrite(&width[i], sizeof(width[i]), 1, file_w);                               
    }                                                                            
  }

  fwrite(&enter, sizeof(enter), 1, file_w);

  for (int i = 0; i < strlen(top); i++) {                                       
    if (top[i] != '\0') {                                                       
      fwrite(&top[i], sizeof(top[i]), 1, file_w);                               
    }                                                                            
  }

  fwrite(&enter, sizeof(enter), 1, file_w);

  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      fwrite(&pxs[i][j], sizeof(pxs[i][j]), 1, file_w);
    }
  }

  fclose(file_w);
}
