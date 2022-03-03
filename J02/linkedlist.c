/*
 * linkedlist.c
 * Author: Grace Choe
 * Date: 3/1/2022
 * 
 * Note:
 * I remade this linkedlist program from Code Jam 2 to make it work correctly
 * in terms of the Code Jam's guidelines. 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct cake {
  float cost;
  char flavor[16];
  struct cake* next;
};

struct cake makeCake(const char* flavor, float cost) {
  struct cake newCake;
  strncpy(newCake.flavor, flavor, 16);
  newCake.cost = cost;
  newCake.next = NULL;
  return newCake;
}

// todo: implement cheapestCake
struct cake* cheapestCake(struct cake* first) {
  struct cake* curr = first;
  struct cake* cheap = first;

  while (curr != NULL) {
    if (curr->cost < cheap ->cost) {
      cheap = curr;
    }
    
    curr = curr->next;
  }

  return cheap;
}

int main() {
  struct cake cake1 = makeCake("red velvet", 2.00);
  struct cake cake2 = makeCake("chocolate", 1.75);
  struct cake cake3 = makeCake("mocha", 3.50);
  struct cake cake4 = makeCake("vanilla", 5.00);
  // todo: define 4th cake

  cake1.next = &cake2;
  cake2.next = &cake3;
  cake3.next = &cake4;
  // draw stack and heap here

  struct cake* current = &cake1;

  while (current != NULL) {
    printf("cake: %s (%.2f)\n", current->flavor, current->cost);
    current = current->next;
  }
  
  // todo: call cheapestCake
  struct cake* cheapest = cheapestCake(&cake1);
  printf("The cheapest cake is %s\n", cheapest->flavor);
}
