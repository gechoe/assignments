//---------------------------------------------------------------------
// sorted_snackbar.c 
// CS223 - Spring 2022
// Ask the user for a list of snacks and store them in alphabetical order
// Name:
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct snack {
  char name[32];
  int quantity;
  float cost;
  struct snack* next;
};

// Insert a new node to a list (implemented as a linked list). 
// The new node should store the given properties
// Param snacks: the first item in the list (NULL if empty)
// Param name: the snack name (max length is 32 characters)
// Param quantity: the snack quantity
// Param cost: the snack cost
// Returns the first item in the list
struct snack* insert_sorted(struct snack* snacks, 
  const char* name, int quantity, float cost) {
    struct snack* new_node = NULL;
  struct snack* current_node = snacks;
  //struct snack* current_node;  

  int val1 = 0;
  int val2 = 0;
  int compare = 0;
  //char lett1, lett2;

  //new_node = (struct snack*)malloc(sizeof(struct snack));
  new_node = malloc(sizeof(struct snack));
  
  if (new_node == NULL) {
    printf("ERROR malloc failed!\n");
    exit(1);
  }  

  strcpy(new_node->name, name);
  new_node->quantity = quantity;
  new_node->cost = cost;
  new_node->next = NULL;

  if (snacks != NULL) {
    while (current_node != NULL) {
      if (strcmp(new_node->name, current_node->name) < 0) {
        new_node->next = current_node;
        snacks = new_node;
        break;
        //current_node = new_node;
      } else if (strcmp(new_node->name, current_node->name) > 0) {
        if (current_node->next != NULL) {
          if (strcmp(new_node->name, current_node->next->name) < 0) {
            new_node->next = current_node->next;
            current_node->next = new_node;
            break; 
          } else {
              current_node = current_node->next;
          }
        } else {
          current_node->next = new_node;
        }
      }
    }
  } else {
    snacks = new_node;
  }
  
  // todo
  return snacks;
  //return NULL;
}

// Delete (e.g. free) all nodes in the given list of snacks
// Param snacks: the first node in the list (NULL if empty)
void clear(struct snack* snacks) {
  struct snack* node = snacks;
  
  while (snacks != NULL) {
    node->next = snacks->next; 
    free(snacks);
    //snacks = NULL;
    snacks = node;
    //if (node != NULL) {
    //  snacks = node;
    //}
  }
  snacks = NULL;

  free(node);
  node = NULL;
}

int main() {
  int num_of_snacks = 0;
  struct snack* head = NULL;
  char name[32];
  int quantity = 0;
  float cost = 0;

//  head = malloc(sizeof(struct node));

  printf("Enter a number of snacks: ");
  scanf("%d", &num_of_snacks);

  for (int i = 0; i < num_of_snacks; i ++) {
    printf("Enter a name: ");
    scanf("%s", name);

    printf("Enter a cost: ");
    scanf("%f", &cost);
    
    printf("Enter a quantity: ");
    scanf("%d", &quantity);
  
    head = insert_sorted(head, name, quantity, cost);
    
  }

  for (int j = 0; j < num_of_snacks; j++) {
    if (head != NULL) {
    printf("%d) %-15s \t cost: $%.2f \t quantity: %-5d\n", j, head->name,
      head->cost, 
      head->quantity);
    }
    head = head->next;
  }
    
  clear(head);
  free(head);
  head = NULL;

  return 0;
}
