/**
 * buddhabrot.c
 * Author: Grace Choe
 * Date: 4/22/2022
 *
 * Description:
 * This program computes and outputs a PPM image of the buddhabrot.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include "read_ppm.h"

//struct for each thread, that way multiple arguents can be taken in pthread_create
struct thread_data {
  //struct ppm_pixel* pal;
  struct ppm_pixel** arr;
  int** barr;
  int** count;
  int sr;
  int er;
  int sc;
  int ec;
};

pthread_mutex_t mutex;
pthread_barrier_t barrier;

//make_array thread's "main" function, computes mandelbrot set portion
void *make_array(void *data) {
  int size = 750;
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
 
  //Step 1: Deteremine mandelbrot set membership
  for (int i = info->sr; i < info->er; i++) {
    for (int j = info->sc; j < info->ec; j++) {
      float xfrac = (float) j / (float) size;
      float yfrac = (float) i / (float) size;
      
      float x0 = xmin + xfrac * (xmax - xmin);
      float y0 = ymin + yfrac * (ymax - ymin);

      float x = 0, y = 0;
      int iter = 0;

      while ((iter < maxIterations) && ((x * x + y * y) < (2 * 2))) {
        xtmp = (x * x) - (y * y) + x0;
        y = 2 * x * y + y0;
        x = xtmp;
        iter++;
      }

      if (iter < maxIterations) { //escaped
        info->barr[i][j] = 0; //membership = false
      } else {
        info->barr[i][j] = 1; //membership = true
      }
    }
  }  

  int max_count = 0;

  //Step 2: Compute visited counts
  for (int i = info->sr; i < info->er; i++) {
    for (int j = info->sc; j < info->ec; j++) {
      if (info->barr[i][j] == 0) {
        float xfrac = (float) j / (float) size;
        float yfrac = (float) i / (float) size;
        
        float x0 = xmin + xfrac * (xmax - xmin);
        float y0 = ymin + yfrac * (ymax - ymin);

        float x = 0, y = 0;

        while (((x * x) + (y * y)) < (2 * 2)) {
          xtmp = (x * x) - (y * y) + x0;
          y = (2 * x * y) + y0;
          x = xtmp;

          int yrow = round(size * (y - ymin) / (ymax - ymin));
          int xcol = round(size * (x - xmin) / (xmax - xmin));
          
          if ((yrow < 0) || (yrow >= size)) continue; //out of range
          if ((xcol < 0) || (xcol >= size)) continue; //out of range
          
          pthread_mutex_lock(&mutex);

          if (size < 200) {
            info->count[yrow][xcol]++;
          } else if (size < 500) {
            info->count[yrow][xcol] += 2;
          } else if (size < 800) {
            info->count[yrow][xcol] += 4;
          } else if (size < 1000) {
            info->count[yrow][xcol] += 6;
          } else if (size < 1300) {
            info->count[yrow][xcol] += 8;
          } else if (size < 1700) {
            info->count[yrow][xcol] += 10;
          } else {
            info->count[yrow][xcol] += 12;
          }
          //info->count[yrow][xcol] += 2;
          max_count++;

          pthread_mutex_unlock(&mutex);
        }
      }
    }
  }

  pthread_barrier_wait(&barrier);

  //Step 3: Compute colors
  float gamma = 0.681;
  float factor = 1.0 / gamma;
    
  for (int p = info->sr; p < info->er; p++) {
    for (int q = info->sc; q < info->ec; q++) {
      float value = 0;

      if (info->count[p][q] > 0) {
        value = log((float)info->count[p][q]) / log((float)max_count);
        value = pow(value, factor);
      }

      pthread_mutex_lock(&mutex);
     
      value = value * 255;

      info->arr[p][q].red = value;
      info->arr[p][q].green = value;
      info->arr[p][q].blue = value;
      
      pthread_mutex_unlock(&mutex);
    }
  }
  
  printf("Thread %lu) finished\n", pthread_self());
  return (void*) NULL;
}

int main(int argc, char* argv[]) {
  int size = 750;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  //Designates the number of thread processes
  int numProcesses = 4;
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
  char name[100] = "buddhabrot-";
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
  strcat(name, ppm);

  // compute image
  //2d array to hold the mandelbrot fractal's pixel values
  struct ppm_pixel **array;
  int **bool_arr;
  int **counts;

  //malloc of 2d array
  array = malloc(sizeof(struct ppm_pixel *) * size);
  for (int i = 0; i < size; i++) {
    array[i] = malloc(sizeof(struct ppm_pixel) * size);
  }

  if (array == NULL) {
    printf("ERROR: malloc failed\n");
    exit(1);
  }

  //malloc of 2d bool_arr
  bool_arr = calloc(size, sizeof(int*));
  for (int j = 0; j < size; j++) {
    bool_arr[j] = calloc(size, sizeof(int));
  }
  
  if (bool_arr == NULL) {
    printf("ERROR: malloc failed\n");
    exit(1);
  }

  //malloc of 2d counts
  counts = calloc(size, sizeof(int*));
  for (int k = 0; k < size; k++) {
    counts[k] = calloc(size, sizeof(int));
  }
  
  //Handles NULL error
  if (counts == NULL) {
    printf("ERROR: malloc failed\n");
    exit(1);
  }
/*
  for (int m = 0; m < size; m++) {
    for (int n = 0; n < size; n++) {
      
      counts[m][n] = 0;
    }
  }
*/
  double timer;
  struct timeval tstart, tend;

  gettimeofday(&tstart, NULL);

  //threads[4], thread IDs
  pthread_t threads[4];
  //info[4] creates the structs for multiple arguments to be passed
  struct thread_data info[4];
  pthread_barrier_init(&barrier, NULL, 4);  
  pthread_mutex_init(&mutex, NULL);
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
    info[i].arr = array;
    info[i].barr = bool_arr;
    info[i].count = counts;
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
  
  pthread_mutex_destroy(&mutex);
  pthread_barrier_destroy(&barrier);

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

    free(bool_arr[m]);
    bool_arr[m] = NULL;

    free(counts[m]);
    counts[m] = NULL; 
  }
  
  //Frees the final outer array
  free(array);
  array = NULL;

  free(bool_arr);
  bool_arr = NULL;

  free(counts);
  counts = NULL;

  return 0;
}
