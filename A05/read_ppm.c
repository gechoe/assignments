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
  int dec, bin, i = 0;
  int *binary = NULL;
  int *bin_array = NULL;
  char s[] = "P6";
  char s2[] = "255";
  char space[] = " ";
  char enter[] = "\n";
  char nums[100];
  binary = malloc(sizeof(int) * 1000);

  if (binary == NULL) {
    printf("Error: malloc failed!\n");
    exit(1);
  }

  bin_array = malloc(sizeof(int) * 1000);                                           
                                                                                 
  if (bin_array == NULL) {                                                          
    printf("Error: malloc failed!\n");                                           
    exit(1);                                                                     
  }

  file_w = fopen(filename, "wb");

  if (file_w == NULL) {
    printf("Cannot find/access file!\n");
    exit(1);
  }
  
  //token = strtok((char*)filename, dash);  
  //char name[] = ".ppm";
  //strcat(token, name);
  
  //printf("%s\n", token);

  /*file_r = fopen(token, "wb");                                                
                                                                                 
  if (file_r == NULL) {                                                          
    printf("Cannot find/access file!\n");                                        
    exit(1);                                                                     
  }*/

  //fgets(s, 100, file_r);
  //s = "P6";
  fwrite(s, sizeof(s), 1, file_w);
  fwrite(space, sizeof(space), 1, file_w); 
  //fprintf(file_w, "\n");

  //fgets(s, 100, file_r);
  //fwrite(s, 1, sizeof(s), file_w);
  
  /*while (s[0] == '#') {
    fgets(s, 100, file_r);
    fwrite(s, 1, sizeof(s), file_w);
  }*/
 
    //fgets(s, 100, file_r);
    sprintf(nums, "%d", w);
    fwrite(&w, sizeof(s), 1, file_w);
    fwrite(space, sizeof(space), 1, file_w);
    //fprintf(file_w, " ");
  
    //fgets(s, 100, file_r);
    sprintf(nums, "%d", h);
    fwrite(&h, sizeof(s), 1, file_w);
    fwrite(enter, sizeof(enter), 1, file_w);
    //fprintf(file_w, "\n");
    
    fwrite(s2, sizeof(s), 1, file_w);

  for (int j = 0; j < h; j++) {
    for (int k = 0; k < w; k++) {
      for (int m = 0; m < 3; m++) {
        if (m == 0) {
          //length = strlen(pxs[j][k].red);
          //printf("%d", pxs[j][k].red);
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
          bin = dec % 2;
          dec /= 2;
  
          //fwrite(&bin, 4, sizeof(dec), file);
          binary[i] = bin;
          i++;
        }
        
        int n = 0;

        for (int j = (i - 1); j >= 0; j--)
        {
            bin_array[j] = binary[n];
            n++;
        }
        
        fwrite(&bin_array, sizeof(bin_array), 1, file_w);
        fprintf(file_w, " ");
        //fwrite(&space, 1, sizeof(space), file); 
        i = 0;
      }
    }
  }

  
  free(binary);
  binary = NULL;
  
  //free(s);
  //s = NULL;

  free(bin_array);
  bin_array = NULL;
  
  //*w = width;
  //*h = height;
  
  //fclose(file_r);
  fclose(file_w);

  //return array;
}
