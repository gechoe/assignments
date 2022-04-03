#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "rand.h"

int main (int argc, char* argv[]) {
  void *init = sbrk(0);                                                          
  printf("The initial top of the heap is %p\n", init);                           
  
  for (int i = 0; i < 10; i++) {
    char *letters;
    
    letters = malloc(sizeof(char) * 100);
    
    free(letters);
  }

  void *current = sbrk(0);                                                       
  printf("The current top of the heap is %p\n", current);                        
                                                                                 
  int increase = current - init;                                                 
  printf("Increased by %d (0x%3X) bytes\n", increase, increase);
    
  return 0 ;
}
