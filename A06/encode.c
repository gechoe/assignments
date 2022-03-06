/**
 * encode.c
 * Author: Grace Choe
 * Date: 3/3/2022
 * 
 * Description:
 * This program reads in a PPM file (raw, or binary, format) and asks the user
 * for a message to embed within it.
 *
 * NOTE: Didn't have time to write individual comments for each code line
 * this time. (OnO) :(
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
  struct ppm_pixel **array;
  int width, height;
  char period[2] = ".";
  char *token;
  char *input = NULL;
  int *all_bin = NULL;
  int dec, mod;
  int value, mask = 1, last_bit;

  if (argc != 2) {
    printf("Usage: ");
    
    for (int i = 0; i < argc; i++) {
      printf("%s ", argv[i]); 
    }
    
    printf("\n");
    exit(1);
  }

  array = read_ppm(argv[1], &width, &height);
  printf("Reading %s with width %d and height %d\n", argv[1], width, height);

  token = strtok(argv[1], period);
  char name[] = "-encoded.ppm";

  strcat(token, name);

  int num = width * height * 3;
  printf("Max number of characters in the image: %d\n", ((num / 8) - 1));

  input = malloc(sizeof(char) * (64 / 8));

  if (input == NULL) {
    printf("Error: Malloc failed!\n");
    exit(1);
  }

  printf("Enter a phrase: ");
  scanf("%s", input);
  
  int len = strlen(input);

  if (len > ((num / 8) - 1)) {
    printf("Invalid! The phrase must be shorter than or equal to the max number of characters!\n");
    exit(1);
  }

  all_bin = malloc(sizeof(int) * (len * 8));

  if (all_bin == NULL) {
    printf("Error: malloc failed!\n");
    exit(1);
  }

  int end_val = 8;

  for (int j = 0; j < len; j ++) {
    dec = input[j];
    
    for (int k = (end_val - 1); k >= (end_val - 8); k--) {
      if (dec == 0) {
        all_bin[k] = 0;
      } else {
        mod = dec % 2;
        all_bin[k] = mod;
        dec /= 2;
      }
    }
  
    end_val += 8;
  }

  int q = 0;

  for (int m = 0; m < height; m++) {
    for (int n = 0; n < width; n++) {
      for (int p = 0; p < 3; p++) {
        if (p == 0) {
          value = array[m][n].red;
        } else if (p == 1) {
          value = array[m][n].green;
        } else {
          value = array[m][n].blue;
        }

        last_bit = value & mask;
       
        if (q < (len * 8)) {
          if (last_bit != all_bin[q]) {
            value = value + 1;
          }
        } else {
          if (last_bit == 1) {
            value = value << 1;
          }
        }

        q++;
 
        if (p == 0) {
          array[m][n].red = value;
        } else if (p == 1) {
          array[m][n].green = value;
        } else {
          array[m][n].blue = value;
        }
      }
    }
  }

  for (int m = 0; m < height; m++) {                                             
    for (int n = 0; n < width; n++) {                                            
      for (int p = 0; p < 3; p++) {                                              
        if (p == 0) {                                                            
          value = array[m][n].red;                                               
        } else if (p == 1) {                                                     
          value = array[m][n].green;                                             
        } else {                                                                 
          value = array[m][n].blue;                                              
        }                                                                        
      }
    }
  }

  write_ppm(token, array, width, height);
  printf("Writing the file %s\n", token);

  free(input);
  input = NULL;

  free(all_bin);
  all_bin = NULL;

  for (int r = 0; r < height; r++) {
    free(array[r]);
    array[r] = NULL;
  }
  
  free(array);
  array = NULL;

  return 0;
}
