/**
 * single_mandelbrot.c
 * Author: Grace Choe
 * Date: 4/6/2022
 *
 * Description:
 * This program writes a mandelbrot png file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"

//make_array function
//Creates the mandelbrot array by filling it with colors
void make_array(struct ppm_pixel** arr, struct ppm_pixel* pal,
  int startrow, int endrow, int startcol, int endcol) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int colorr, colorg, colorb, black = 0;
  float xtmp = 0;

  //For loop within for loop to assign red, green, blue pixel colors for each
  //pixel
  for (int r = startrow; r < endrow; r++) {
    for (int c = startcol; c < endcol; c++) {
      float xfrac = (float) c / (float)size;
      float yfrac = (float) r / (float)size;
        
      float x0 = xmin + xfrac * (xmax - xmin);
      float y0 = ymin + yfrac * (ymax - ymin);
      float x = 0, y = 0;
      int iter = 0;

      //while loop creates the fractal like image by assigning a certain iter
      //value which is then used to compute the pixel colors
      while ((iter < maxIterations) && ((x * x + y * y) < (2 * 2))) {
        xtmp = (x * x) - (y * y) + x0;
        y = (2 * x * y) + y0;
        x = xtmp;
        iter++;
      }

      //Computes the colors for red, green, and blue
      if (iter < maxIterations) {
        colorr = pal[iter].red;
        colorg = pal[iter].green;
        colorb = pal[iter].blue;
      } else {
        colorr = black;
        colorg = black;
        colorb = black;
      }
      
      arr[r][c].red = colorr;
      arr[r][c].green = colorg;
      arr[r][c].blue = colorb;
    }
  }
}

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
  // generate palette
  //Sets a random seed to ensure that the color palette is different each time
  srand(time(0));  
  
  //palette array to hold palette colors
  struct ppm_pixel *palette;
  palette = malloc(sizeof(struct ppm_pixel *) * (maxIterations + 20));
  
  if (palette == NULL) {
    printf("ERROR: malloc failed\n");
    exit(1);
  }

  //Sets the palette up with certain colors
  for (int q = 0; q < maxIterations; q++) {
    int red = rand() % 255;
    int green = rand() % 255;
    int blue = rand() % 255;
    
    palette[q].red = red;
    palette[q].green = green;
    palette[q].blue = blue; 
  }

  // compute image
  //2d array to hold the mandelbrot fractal's pixel values
  struct ppm_pixel **image_array;

  //malloc of the array
  image_array = malloc(sizeof(struct ppm_pixel *) * size);
  
  for (int i = 0; i < size; i++) {
    image_array[i] = malloc(sizeof(struct ppm_pixel) * size);
  }

  if (image_array == NULL) {
    printf("ERROR: malloc failed\n");
    exit(1);
  }

  double timer;
  struct timeval tstart, tend;

  //The start time, gets the time for before the mandelbrot (array/png) is made
  gettimeofday(&tstart, NULL);

  //Uses the make_array function to make mandelbrot color array
  make_array(image_array, palette, 0, size, 0, size);

  //The end time, gets the time for after the mandelbrot array is made
  gettimeofday(&tend, NULL);

  //name array holds the created name of the file
  char name[1024];
  snprintf(name, 1024, "mandelbrot-%d-%lu.ppm", size, time(0));
  //Write the computed mandelbrot into a png file
  write_ppm(name, image_array, size, size);
  
  //timer, calculates the total time the process took
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Computed mandelbrot set (%dx%d) in %g seconds\n", size, size, timer);
  printf("Writing file: %s\n", name);

  //Frees the arrays within the array (2D array)
  for (int m = 0; m < size; m++) {
    free(image_array[m]);
    image_array[m] = NULL;
  }
  
  //Frees the final outer array
  free(image_array);
  image_array = NULL;

  //Frees the palette array
  free(palette);
  palette = NULL;

  return 0;
}
