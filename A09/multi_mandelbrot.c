#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "read_ppm.h"

struct ppm_pixel** make_array(struct ppm_pixel* pal, struct ppm_pixel** arr, 
  int startr, int endr, int startc, int endc) {
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
  for (int j = startr; j < endr; j++) {
    for (int k = startc; k < endc; k++) {
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
        colorr = pal[iter].red;
        colorg = pal[iter].green;
        colorb = pal[iter].blue;
      } else {
        colorr = black;
        colorg = black;
        colorb = black;
      }

      //write color to image at location (row, col)
      arr[j][k].red = colorr;
      arr[j][k].green = colorg;
      arr[j][k].blue = colorb;
    }
  }
 
  return arr;
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
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your code here
  char name[200] = "multi-mandelbrot-";
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

  // compute image
  //2d array to hold the mandelbrot fractal's pixel values
  //struct ppm_pixel **array;
 
  //Creating space for shared memory to hold 2d array for child processes 
  int shmid, shmid2;
  shmid = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel*) * (size + 10), 0644 | IPC_CREAT);

  if (shmid == -1) {
    perror("Error: Cannot initialize shared memory!\n");
    exit(1);
  }
  
  struct ppm_pixel** array = shmat(shmid, NULL, 0);
  
  if (*array == (void*) -1) {
    perror("Error: cannot access shared memory\n");
    exit(1);
  }

  for (int w = 0; w < size; w++) {
    shmid2 = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel) * (size + 10), 0644 | IPC_CREAT);
    array[w] = shmat(shmid2, NULL, 0);
    if (shmid == -1) {
      perror("Error: Cannot initialize shared memory!\n");
      exit(1);
    }
  }
  
  if (array == (void*) -1) {
    perror("Error: cannot access shared memory\n");
    exit(1);
  }
  
  //double timer;
  struct timeval tstart, tend;

  //The start time, gets the time for before the mandelbrot (array/png) is made
  gettimeofday(&tstart, NULL);

  int colorr, colorg, colorb, black = 0;
  float xtmp = 0;

  int startr, endr, startc, endc;

  for (int y = 0; y < 4; y++) {
    int pid = fork();
    
    if (pid == -1) {
      printf("Error: fork failed\n");
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
      array = make_array(palette, array, startr, endr, startc, endc);
      exit(0);
    } else {
      printf("Launched child process: %d\n", pid);
      printf("%d) Sub-image block: cols (%d, %d) to rows (%d, %d)\n",
        pid, startr, endr, startc, endc);
    }
    //printf("%d) Sub-image block: cols (%d, %d) to rows (%d, %d)\n",
      //pid, startr, endr, startc, endc);
  }

  for (int z = 0; z < 4; z++) {
    int status;
    int pid = wait(&status);
    
    printf("Child process complete: %d\n", pid);
  }

  //The end time, gets the time for after the mandelbrot array is made
  gettimeofday(&tend, NULL);

  //Write the computed mandelbrot into a png file
  write_ppm(name, array, size, size);
  
  for (int m = 0; m < size; m++) {
    if (shmdt(array[m]) == -1) {
      perror("Error: Cannot detach from shared memory!\n");
      //exit(1);
    }/* else if (shmdt(array[m]) == 0) {
      if (shmctl(shmid2, IPC_RMID, 0) == -1) {
      perror("nott Error: Cannot remove shared memory!\n");
      //exit(1);
      }
    }*/
  }

  for (int n = 0; n < (size + 10); n++) {
    //shmctl(shmid2, IPC_RMID, 0);

    if (shmctl(shmid2, IPC_RMID, 0) == -1) {
    perror("nott Error: Cannot remove shared memory!\n");
    //exit(1);
    }

  }

  if(shmdt(array) == -1) {
    perror("huh Error: Cannot detach from shared memory!\n");
    //exit(1);
  }

  if (shmctl(shmid, IPC_RMID, 0) == -1) {
    perror("ok Error: Cannot remove shared memory!\n");
    //exit(1);
  }

  //timer, calculates the total time the process took
  double timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Computer mandelbrot set (%dx%d) in %g seconds\n", size, size, timer);
  printf("Writing file: %s\n", name);
  
  //Frees the palette array
  free(palette);
  palette = NULL;

  return 0;
}
