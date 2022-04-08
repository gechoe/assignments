/**
 * single_mandelbrot.c
 * Author: Grace Choe
 * Date: 4/6/2022
 *
 * Description:
 * This program writes a mandelbrot ppm file. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your work here
  //Creates file name  
  char name[100] = "mandelbrot-";
  char *dash = "-";
  char sizelet[4];
  sprintf(sizelet, "%d", size);
  char png[] = ".png";
  time_t timenow = time(0);
  char current[11];
  sprintf(current, "%d", (int)timenow);

  printf("size: %s  current: %s\n", sizelet, current);

  strcat(name, sizelet);
  strcat(name, dash);
  strcat(name, current);
  strcat(name, png);
 
  // generate pallet
  srand(time(0));  
  
  struct ppm_pixel *palette;
  palette = malloc(sizeof(struct ppm_pixel *) * (maxIterations + 20));
  
  if (palette == NULL) {
    printf("ERROR: malloc failed\n");
    exit(1);
  }

  for (int q = 0; q < maxIterations; q++) {
    int red = rand() % 255;
    int green = rand() % 255;
    int blue = rand() % 255;
    
    palette[q].red = red;
    palette[q].green = green;
    palette[q].blue = blue; 
  }

  double timer;
  struct timeval tstart, tend;

  //The start time, gets the time for before the mandelbrot (array/png) is made
  gettimeofday(&tstart, NULL);

  // compute image
  struct ppm_pixel **array;

  array = malloc(sizeof(struct ppm_pixel *) * size);
  
  for (int i = 0; i < size; i++) {
    array[i] = malloc(sizeof(struct ppm_pixel) * size);
  }

  if (array == NULL) {
    printf("ERROR: malloc failed\n");
    exit(1);
  }

  int colorr, colorg, colorb, black = 0;
  float xtmp = 0; //x = 0, y = 0;

  for (int j = 0; j < size; j++) {
    for (int k = 0; k < size; k++) {
      //should this be size / (size * size)?
      float xfrac = (float)size / (float)size;
      float yfrac = (float)size / (float)size;
      
      float x0 = xmin + xfrac * (xmax - xmin);
      float y0 = ymin + yfrac * (ymax - ymin);
      float x = 0, y = 0;
      int iter = 0;
      
      while ((iter < maxIterations) && ((x * x + y * y) < (2 * 2))) {
        //float xtmp 
        xtmp = (x * x) - (y * y) + x0;
        y = (2 * x * y) + y0;
        x = xtmp;
        //printf("iter: %d  x: %f   y: %f\n", iter, x, y);
        iter = iter + 1;
        //iter++;
        printf("iter: %d  x: %f   y: %f\n", iter, x, y);
      }

      if (iter < maxIterations) {//escaped
        //array[j][k].red = palette[iter].red;
        //array[j][k].green = palette[iter].green;
        //array[j][k].blue = palette[iter].blue;
        colorr = palette[iter].red;
        colorg = palette[iter].green;
        colorb = palette[iter].blue;
      } else {
        //array[j][k].red = black;
        //array[j][k].green = black;
        //array[j][k].blue = black;
        colorr = black;
        colorg = black;
        colorb = black;
      }

      array[j][k].red = colorr;
      array[j][k].green = colorg;
      array[j][k].blue = colorb;
      //write color to image at location (row, col)
      //array[j][k] = (struct ppm_pixel)color; 
    }
  }

  write_ppm(name, array, size, size);
  
  //The end time, gets the time for after the mandelbrot (array/png) is made
  gettimeofday(&tend, NULL);

  //timer, calculates the total time the process took
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Computer mandelbrot set (%dx%d) in %g seconds\n", size, size, timer);
  printf("Writing file: %s\n", name);

  //Frees the arrays within the array (2D array)
  for (int m = 0; m < size; m++) {
    free(array[m]);
    array[m] = NULL;
  }
  
  //Frees the final outer array
  free(array);
  array = NULL;

  free(palette);
  palette = NULL;

  return 0;
}
