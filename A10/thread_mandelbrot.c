/**
 * thread_mandelbrot.c
 * Author: Grace Choe
 * Date: 4/22/2022
 *
 * Description:
 * This program computes and outputs a PPM image of the mandelbrot set using
 * threads.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include "read_ppm.h"

//struct for each thread, that way multiple arguents can be taken in pthread_create
struct thread_data {
  struct ppm_pixel* pal;
  struct ppm_pixel** arr;
  int sr;
  int er;
  int sc;
  int ec;
};

//make_array thread's "main" function, computes mandelbrot set portion
void *make_array(void *data) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int colorr, colorg, colorb, black = 0;
  float xtmp = 0;
  
  struct thread_data *info = (struct thread_data *) data;
  
  printf("Thread %lu) sub-image block: cols (%d, %d) to rows (%d, %d)\n",
    pthread_self(), info->sr, info->er, info->sc, info->ec);
  
  //For loop within for loop to assign red, green, blue pixel colors for each
  //pixel
  for (int j = info->sr; j < info->er; j++) {
    for (int k = info->sc; k < info->ec; k++) {
      float xfrac = (float) k / (float)size;
      float yfrac = (float) j / (float)size;
      
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
        colorr = info->pal[iter].red;
        colorg = info->pal[iter].green;
        colorb = info->pal[iter].blue;
      } else {
        colorr = black;
        colorg = black;
        colorb = black;
      }

      //write color to image at location (row, col)
      info->arr[j][k].red = colorr;
      info->arr[j][k].green = colorg;
      info->arr[j][k].blue = colorb;
    }
  }
 
  printf("Thread %lu) finished\n", pthread_self());
  return (void*) NULL;
}

//main function
int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  //Designates the number of thread processes.
  int numProcesses = 4;
  //Boundaries for each thread's mandelbrot creation.
  int startr, endr, startc, endc;
  
  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> "
        "-b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your code here
  //file name parts
  char name[100] = "mandelbrot-";
  char *dash = "-";
  char sizelet[100];
  sprintf(sizelet, "%d", size);
  char ppm[] = ".ppm";
  time_t timenow = time(0);
  char current[11];
  sprintf(current, "%d", (int)timenow);

  //Creates file name
  strcat(name, sizelet);
  strcat(name, dash);
  strcat(name, current);
  strcat(name, png);
 
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
  struct ppm_pixel **array;

  //malloc of the array
  array = malloc(sizeof(struct ppm_pixel *) * size);
  
  for (int y = 0; y < size; y++) {
    array[y] = malloc(sizeof(struct ppm_pixel) * size);
    memset(array[y], (int)'\0', size);
  }

  if (array == NULL) {
    printf("ERROR: malloc failed\n");
    exit(1);
  }

  double timer;
  struct timeval tstart, tend;

  //The start time, gets the time for before the mandelbrot (array/png) is made
  gettimeofday(&tstart, NULL);

  int start = 0, end = 480;
  
  //threads[4], thread IDs
  pthread_t threads[4];
  //info[4] creates the structs for multiple arguments to be passed
  struct thread_data info[4];
  
  //for loop for threads to be created and executed.
  for (int i = 0; i < 4; i++) {
    if (i == 0) {
      startr = 0, endr = (size / 2), startc = 0, endc = (size / 2);
    } else if (i == 1) {
      startr = (size / 2), endr = size, startc = 0, endc = (size / 2);
    } else if (i == 2) {
      startr = 0, endr = (size / 2), startc = (size / 2), endc = size;
    } else {
      startr = (size / 2), endr = size, startc = (size / 2), endc = size;
    }

    //sets up all the multiple arguments values
    info[i].pal = palette;
    info[i].arr = array;
    info[i].sr = startr;
    info[i].er = endr;
    info[i].sc = startc;
    info[i].ec = endc;

    //Creates thread
    pthread_create(&threads[i], NULL, make_array, (void*) &info[i]);
  }

  for (int z = 0; z < 4; z++) {
    //Joins threads
    pthread_join(threads[z], NULL);
  }
  
  //The end time, gets the time for after the mandelbrot array is made
  gettimeofday(&tend, NULL);

  //Write the computed mandelbrot into a png file
  write_ppm(name, array, size, size);
  
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

  //Frees the palette array
  free(palette);
  palette = NULL;

  return 0;
}
