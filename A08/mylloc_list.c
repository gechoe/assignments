#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct chunk {
  int size;
  int mem_used;
  struct chunk *next;
};
struct chunk *flist = NULL;

void *malloc (size_t size) {
  if (size == 0) {
    return NULL;
  }

  struct chunk *next = flist;
  struct chunk *prev = NULL;

  while (next != NULL) {
    if (next -> size >= size) {
      if (prev != NULL) {
        prev->next = next->next;
      } else {
        flist = next->next;
      }
      return (void*)(next + 1);
    } else {
      prev = next;
      next = next -> next;
    }
  }

  void *memory = sbrk(size + sizeof(struct chunk));
  if (memory == (void *) - 1) {
    return NULL;
  } else {
    struct chunk *cnk = (struct chunk*)memory;
    cnk->size = size;
    cnk->mem_used = size;
    return (void *)(cnk + 1);
  }
}

void free(void *memory) {
  if (memory != NULL) {
    struct chunk *cnk = (struct chunk*)((struct chunk*)memory -1);
    cnk->next = flist;
    flist = cnk;
  }
  return;
}

void fragstats(void* buffers[], int len) {
  //Total blocks for free chunks and in-use chunks
  int total_usedc = 0, total_freec = 0;
  //The largest and smallest unused memory across all used chunks
  int small_unuse = 0, large_unuse = 0;
  //The total unused memory across all used chunks and unused_mem for each chunk
  int total_unuse = 0, unuse = 0;
  //The largest and smallest sizes across all free chunks
  int small_free = 0, large_free = 0;

  struct chunk *cnk;

  //for loop
  //Checks all used chunks to find unused then unused's smallest, largest,
  //total, etc.
  for (int k = 0; k < len; k++) {
    if (buffers[k] != NULL) {
      total_usedc++;
      cnk = (struct chunk*)(buffers[k] - 1);
      //finds unused using used and size
      unuse = (cnk->size) - (cnk->mem_used);
      total_unuse = total_unuse + unuse;

      //if statement, to find the largest unused
      if (unuse > large_unuse) {
        large_unuse = unuse;
      }

      //if statement to find the smallest unused
      if ((small_unuse == 0) || (unuse < small_unuse)) {
        small_unuse = unuse;
      }
    }
  }

  //The average unused memory across all used chunks
  float aver_unuse = total_unuse / total_usedc;

  struct chunk *fl = flist;
  int total_free = 0;

  //while loop
  //Checks all free chunks to find smallest, largest, total, etc.
  while (fl != NULL) {
    total_freec++;
    total_free = total_free + fl->size;

    //if statement to find the largest free
    if (fl->size > large_free) {
      large_free = fl->size;
    }

    //if statement to find the smallest free
    if ((small_free == 0) || (fl->size < small_free)) {
      small_free = fl->size;
    }

    fl = fl->next;
  }

  //The average sizes os all free chunks  
  float aver_free = total_free / total_freec;

  //The amount of total_blocks (free chunks + used chunks)
  int total_blocks = total_usedc + total_freec;

  printf("Total blocks: %d Free: %d Used: %d\n", total_blocks, total_freec,
    total_usedc);
  printf("Internal unused: total: %d average: %.1f smallest: %d largest: %d\n",
    total_unuse, aver_unuse, small_unuse, large_unuse);
  printf("External unused: total: %d average: %.1f smallest: %d largest: %d\n",
    total_free, aver_free, small_free, large_free);
}
