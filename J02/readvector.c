/*
 * readvector.c
 * Author: Grace Choe
 * Date: 3/1/2022
 * 
 * Note:
 * I remade this readvector program from Code Jam 2 to make it work correctly
 * in terms of the Code Jam's guidelines. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// readvector opens and reads the given file
// The size of the vector should be stored in the parameter size
// The values should be returned in a flat float array
float* readvector(const char* filename, int *size) {
  // todo: implement me
  float *f_array = NULL;
  
  FILE *file;

  file = fopen(filename, "r");

  if (file == NULL) {
    printf("Error: Unable to open file %s\n", filename);
    exit(1);
  }

  fscanf(file, "%d", size);

  f_array = malloc(sizeof(float) * (*size));

  if (f_array == NULL) {
    printf("Error: malloc failed!\n");
    exit(1);
  }
 
  for (int i = 0; i < *size; i++) {
    f_array[i] = '\0';
    fscanf(file, "%f", &f_array[i]);
  }
  
  fclose(file);

  return f_array;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: readvector <filename>\n");
    exit(0);
  }

  int size = 0;
  float* vector = readvector(argv[1], &size);

  // todo: print vector values
  for (int i = 0; i < size; i++) {
    if (vector[i] != '\0') {
      printf("%.6f\n", vector[i]);
    }
  }

  free(vector);
  vector = NULL;

  return 0;
}
