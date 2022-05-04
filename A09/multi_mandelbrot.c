/**
 * multi_mandelbrot.c
 * Author: Grace Choe
 * Date: 4/15/2022
 *
 * Description:
 * This program writes a mandelbrot png file using multiple (four) child 
 * processes using fork.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
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

  free(pal);
  pal = NULL;

  free(arr);
  arr = NULL;
}

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;

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

  printf("Generating image with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your code here
  // generate pallet
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

  //shared memory initialized
  int shmid;
  shmid = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel) * size *size, 0644 | IPC_CREAT);
  if (shmid == -1) {
    perror("Error: cannot initialize shared memory\n");
    exit(1);
  }

  //shared memory accessed
  struct ppm_pixel* buffer = shmat(shmid, NULL, 0);
  if (buffer == (void*) -1) {
    perror("Error: cannot access shared memory\n");
    exit(1);
  } 

  //Creates 2d array using the shared memory space
  struct ppm_pixel** image_array = malloc(sizeof(struct ppm_pixel*) * size);
  for (int i = 0; i < size; i++) {
    image_array[i] = &(buffer[i*size]); 
  }

  //timer
  struct timeval tstart, tend;
  int startr, endr, startc, endc;

  //The start time, gets the time for before the mandelbrot (array/png) is made
  gettimeofday(&tstart, NULL);

  //for loop to create four forked child processes which create the mandelbrot
  for (int y = 0; y < 4; y++) {
    int pid = fork();

    if (pid == -1) {
      printf("Error: Fork failed!\n");
      exit(pid);
    }

    if (y == 0) {
      startr = 0, endr = (size / 2), startc = 0, endc = (size / 2);
    } else if (y == 1) {
      startr = (size / 2), endr = size, startc = 0, endc = (size / 2);
    } else if (y == 2) {
      startr = 0, endr = (size / 2), startc = (size / 2), endc = size;
    } else {
      startr = (size / 2), endr = size, startc = (size / 2), endc = size;
    }

    if (pid == 0) {
      make_array(image_array, palette, startr, endr, startc, endc);
      exit(0);
    } else {
      printf("Launched child process: %d\n", pid);
      printf("%d) Sub-image block: cols (%d, %d) to rows (%d, %d)\n",
        pid, startr, endr, startc, endc);
    }
  }
  
  for (int z = 0; z < 4; z++) {
    int status;
    int pid = wait(&status);
    
    printf("Child process complete: %d\n", pid);
  }

  //The end time, gets the time for after the mandelbrot array is made
  gettimeofday(&tend, NULL);

  //name array holds the created name of the file
  char name[1024];
  snprintf(name, 1024, "multi-mandelbrot-%d-%lu.ppm", size, time(0));
  write_ppm(name, image_array, size, size);
  printf("Writing file: %s\n", name);
  
  //frees image_array
  free(image_array);
  image_array = NULL;

  //Makes sure shared memory is detached
  if (shmdt(buffer) == -1) {
    perror("Error: cannot detatch from shared memory\n");
    exit(1);
  }

  //Makes sure shared memory is removed
  if (shmctl(shmid, IPC_RMID, 0) == -1) {
    perror("Error: cannot remove shared memory\n");
    exit(1);
  }

  //frees palette
  free(palette);
  palette = NULL;
}
