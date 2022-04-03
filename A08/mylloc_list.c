#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct chunk {
  int size;
  int mem_used;
  struct chunk *next;
};

struct chunk *flist = NULL;
int total = 1, ub = 0;

void *malloc (size_t size) {
  if (size == 0){
    return NULL;
  }

  struct chunk *next = flist;
  struct chunk *prev = NULL;

  while (next != NULL) {
    total++;

    if (next->size >= size) {
      if (prev != NULL) {
        next->mem_used = size; /////////
        prev->next = next->next;
      } else {
        flist = next->next;
      }
      return (void*)(next + 1);
    } else {
      prev = next;
      next = next->next;
    }
  }

  /*use sbrk to allocate new memory*/
  void *memory = sbrk(size + sizeof(struct chunk));
  if (memory == (void *) -1) {
    return NULL;
  } else {
    struct chunk* cnk = (struct chunk*) memory;
    cnk->size = size;
    cnk->mem_used = size;
    
    return (void*)(cnk + 1);
  }
}

void free(void *memory) {
  if (memory != NULL) {
   /*we're jumping back one chunk position*/
    struct chunk *cnk = (struct chunk*)((struct chunk*)memory - 1);
    cnk->next = flist;
    cnk->mem_used = 0;
    flist = cnk;
    //cnk->mem_used = 0;
  }

  return;
}

void fragstats(void* buffers[], int len) {
  //void *init = sbrk(0);
  struct chunk *cnk = flist;  
  //struct chunk *smallest;
  //struct chunk *largest;
  //int i = 0;

  int total = 0, used = 0;
  int largest, smallest;
  int i = 0;
  int unt = 0, unu = 0;

  while (cnk != NULL) {
    i++;
    //int x = (cnk->size);
    //int x = *(buffers[i]->size);
    total = total + (cnk->size);
    used = used + (cnk->mem_used);
    
    int t = cnk->size;
    int u = cnk->mem_used;
    int un = t - u;
    
    if (un >= unu) {
      largest = un;
      smallest = unu;
    } else {
      largest = unu;
      smallest = un;
    }
    
    unu = un;
    unt += un;
    
    cnk = cnk->next;
  }
  
  int unused = total - used;
  float av = unt / i;  

  printf("unused memory total: %d %d, average: %.1f, smallest: %d, largest: %d\n",
     unt, unused, av, smallest, largest);
  printf("total: %d\n", total);
  //printf("%ld  %d  %d\n", sizeof(*(buffers)), len, tb);  

/*  void *cnk = flist;
  //t = total, ft = freed total, ut = used total
  int t = 0, ft = 0, ut = 0;
  //it = increased total, i

  for (int i = 0; i < len; i++) {
    t += buffers[i]->size;
    ut += cnk->mem_used;
  }

  //external linked list
  //internal buffers
  while (cnk != NULL) {
    t += cnk->size;
    ut += cnk->mem_used;
  }

  ft = t - ut;
   
  printf("Total blocks: %d Free: %d Used: %d\n", t, ft, ut); 
*/}
