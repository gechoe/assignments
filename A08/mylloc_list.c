#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct chunk {
  int size;
  int mem_used;
  struct chunk *next;
};

struct chunk *flist = NULL;
int total = 0, freed = 0, used = 0;

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
        next->mem_used = size;
        prev->next = next->next;
        freed++;
      } else {
        flist = next->next;
        used++;
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
    freed++;
    cnk->mem_used = 0;
  }

  return;
}

void fragstats(void* buffers[], int len) {
  int l = 0, mem = 0, memm, mem_total = 0, large, small;

  for (int k = 0; k < len; k++) {
    if (buffers[k]) {
      l++;
      mem = *((int*)buffers[k]);
      mem_total += mem;
      
      if (k == 0) {
        memm = mem;
      }
      if (mem < memm) {
        if (memm >= large) {
          large = memm;
        }
        if (mem <= small) {
          small = mem;
        }
      } else if (mem > memm) {
        if (mem >= large) {
          large = mem;
        }
        if (memm <= small) {
          small = memm;
        }
      } else {
        small = mem;
        large = mem;
      }
    }
    memm = mem;
  }
  float aver = mem_total / l;
 
  struct chunk *cnk = flist, *prev = NULL;
  int largest, smallest, i = 0, unt = 0, unu;

  while (cnk != NULL) {
    i++;
    
    int t = cnk->size;
    int u = cnk->mem_used;
    int un = t - u;

    if (prev != NULL) {
      if (un <= unu) {
        if (unu >= largest) {
          largest = unu;
        }
        if (un <= smallest) {
          smallest = un;
        }
      } else {
        if (un >= largest) {
          largest = un;
        }
        if (unu <= smallest) {
          smallest = unu;
        }
      }
    } else {
      smallest = un;
      largest = un;
    }

    unu = un;
    unt += un;
    
    prev = cnk;
    cnk = cnk->next;
  }
  
  float av = unt / i;  
  printf("Total blocks: %d Free: %d Used: %d\n", (i + l), i, l);
  printf("External unused: total: %d, average: %.1f, smallest: %d, largest: %d\n",
     mem_total, aver, small, large);
  printf("Internal unused: total: %d, average: %.1f, smallest: %d, largest: %d\n",
     unt, av, smallest, largest);
}
