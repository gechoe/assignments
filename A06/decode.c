/**
 * decode.c
 * Author: Grace Choe
 * Date: 3/2/2022
 * 
 * Description:
 * This program reads in a PPM file (raw, or binary, format) and then outputs
 * any message that might be stored in the least significant bits of each color. 
 * 
 * NOTE: Didn't have time to write individual comments for each code line
 * this time. (OnO) :(
 */

#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int power(int base, int exponent) {
  int answer = 1;

  if (exponent > 0) {
    for (int i = 0; i < exponent; i++) {
      answer *= base;
    }
  }

  return answer;
}

int main(int argc, char** argv) {
  struct ppm_pixel **array;
  int width, height;
  int value, mask = 1, last_bit;
  int *all_bin = NULL;
  int dec, dec_fin = 0;
  char *string = NULL;

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
  
  int num = width * height * 3;
  printf("Max number of characters in the image: %d\n", (num / 8));
  
  all_bin = malloc(sizeof(int) * (num));

  if (all_bin == NULL) {
    printf("Error: malloc failed!\n");
    exit(1);
  }

  string = malloc(sizeof(char) * (num / 8));

  if (string == NULL) {
    printf("Error: malloc failed!\n");
    exit(1);
  }

  int n = 0;

  for (int j = 0; j < height; j++) {
    for (int k = 0; k < width; k++) {
      for (int m = 0; m < 3; m++) {
        if (m == 0) {
          value = array[j][k].red;
        } else if (m == 1) {
          value = array[j][k].green;
        } else {
          value = array[j][k].blue;
        }

        last_bit = value & mask;
        all_bin[n] = last_bit;
        n++;         
      }
    }
  }

  int least = 0, most = 8;
  int q = 0, exp = 7;

  for (int p = 0; (least <= p) && (p < most); p++) {
    if (all_bin[p] == 1)  {
      dec = power(2, exp);
      dec_fin += dec;
    }
   
    exp--;
    char c = dec_fin;

    if (p == (most - 1)) {
      string[q] = c;
      
      if (string[q] == '\0') {
        break;
      }
      
      q++;
      dec_fin = 0;
      exp = 7;

      if ((most + 8) <= n) {
        least += 8;
        most += 8;
      }
    }
  }
  
  printf("%s\n", string);

  free(all_bin);
  all_bin = NULL;

  free(string);
  string = NULL;

  for (int r = 0; r < height; r++) {
    free(array[r]);
    array[r] = NULL;
  }

  free(array);
  array = NULL;

  return 0;
}
