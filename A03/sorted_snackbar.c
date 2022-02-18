//---------------------------------------------------------------------
// sorted_snackbar.c 
// CS223 - Spring 2022
// Ask the user for a list of snacks and store them in alphabetical order
// Name: Grace Choe
// Date: 2/10/2022
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//snack struct types defined with name, cost, quantity, and a next pointer.
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
struct snack* insert_sorted(struct snack* snacks, const char* name,
  int quantity, float cost) {
  struct snack* new_node = NULL;
  struct snack* current_node = snacks;

  //Mallocs new_node, new_node being the new element added to the linked list.
  new_node = malloc(sizeof(struct snack));

  //Checks return value and handles error.
  if (new_node == NULL) {
    printf("ERROR malloc failed!\n");
    exit(1);
  }

  //Gives new_node properties based on function param.
  strcpy(new_node->name, name);
  new_node->quantity = quantity;
  new_node->cost = cost;
  new_node->next = NULL;

  //Combination of if and while statements to alphabetize and insert the new_node
  //at its appropriate location in the linked list.
  if (snacks != NULL) {
    //current_node is first set equal to snacks (the head node), but then
    //traverses through the linked list to find the appropriate place to insert
    //new_node.
    /*if ((strcmp(new_node->name, current_node->name) < 0) {
      if () {}
    }*/
    while (current_node != NULL) {
      //new_node is placed before the current_node (head).
      if (strcmp(new_node->name, current_node->name) < 0) {
        new_node->next = current_node;
        snacks = new_node;
        break;
      } else if (strcmp(new_node->name, current_node->name) > 0) {
        //Insertion of new_node between two linked list nodes.
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
          break;
        }
      }
    }
  } else {
    snacks = new_node;
  }

  return snacks;
}

// Delete (e.g. free) all nodes in the given list of snacks
// Param snacks: the first node in the list (NULL if empty)
void clear(struct snack* snacks) {
  struct snack* node;

  //Keeps freeing head as long as head is not null.
  while (snacks != NULL) {
    node = snacks->next;

    free(snacks);
    //Moves head to the next node after freeing the first one.
    snacks = node;
  }
  snacks = NULL;

  //Frees and cleans up node to null;
  free(node);
  node = NULL;
}

//main function
int main() {
  int num_of_snacks = 0;
  struct snack *head = NULL;
  char name[32];
  int quantity = 0;
  float cost = 0;

  //Asks for user input on number of snacks, adn then the snacks.
  printf("Enter a number of snacks: ");
  scanf("%d", &num_of_snacks);
  printf("\n");

  for (int i = 0; i < num_of_snacks; i ++) {
    printf("Enter a name: ");
    scanf("%s", name);

    printf("Enter a cost: ");
    scanf("%f", &cost);

    printf("Enter a quantity: ");
    scanf("%d", &quantity);

    //Puts the snack(s) one at a time into the linked list.
    head = insert_sorted(head, name, quantity, cost);

    printf("\n");
  }

  struct snack* holder = head;

  printf("Welcome to Grace's GrOoVy Snack Bar! ~(OuO)~\n\n");

  //Prints out the snacks in alphabetical order.
  for (int j = 0; j < num_of_snacks; j++) {
    if (holder != NULL) {
      printf("%d) %-15s \t cost: $%.2f \t quantity: %-5d\n", j, holder->name,
      holder->cost, holder->quantity);
    }
    holder = holder->next;
  }

  //Clears/frees and nulls everything.
  clear(head);

  return 0;
}
