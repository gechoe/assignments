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
  FILE *file;
  int dec;//, bin;//, i = 0; //,length;
  int *binary = NULL;
  char *s = NULL;

  s = malloc(sizeof(char) * 100);

  if (s == NULL) {
    printf("Error: Malloc failed!\n");
    exit(1);
  }

  binary = malloc(sizeof(int) * 1000);

  if (binary == NULL) {
    printf("Error: malloc failed!\n");
    exit(1);
  }

  file = fopen(filename, "wb");

  if (file == NULL) {
    printf("Cannot find/access file!\n");
    exit(1);
  }

  //fgets(s, 100, file);
  //fgets(s, 100, file);
  
  //while (s[0] == '#') {
    //fgets(s, 100, file);
  //}
 
  //fgets(s, 100, file);
  //fgets(s, 100, file);

  for (int j = 0; j < h; j++) {
    for (int k = 0; k < w; k++) {
      for (int m = 0; m < 3; m++) {
        if (m == 0) {
          //length = strlen(pxs[j][k].red);
          dec = (pxs[j][k].red) - '0';
        } else if (m == 1) {
          //length = strlen(pxs[j][k].green);
          dec = (pxs[j][k].green) - '0';
        } else {
          //length = strlen(pxs[j][k].blue);
          dec = (pxs[j][k].blue) - '0';
        }
      //dec = pxs[j][k] - '0';
        
        //binary = malloc(sizeof(int) * 1000);

        while (dec > 0) {
          int bin = dec % 2;
          dec /= 2;
  
          fwrite(&bin, 4, sizeof(dec), file);
          //binary[i] = bin;
          //i++;
        }
        
        //fwrite(&binary, 4, sizeof(binary), file);
        
        //i = 0;
      }
    }
  }

  
  free(binary);
  binary = NULL;
  
  free(s);
  s = NULL;

  //free(string);
  //string = NULL;
  
  //*w = width;
  //*h = height;
  
  fclose(file);

  //return array;
}
