/**
 * dynamic_snackbar.c
 * Author: Grace Choe
 * Date: 2/3/2022
 *
 * Description:
 * This program allows the user to add snacks to the snackbar, featuring (1) the
 * ability to add a new snack to the list of snacks and (2) the ability to print
 * out the current list of snacks.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//snack struct type defined with name, cost, quantity.
struct snack {
  char name[32];
  float cost;
  int quantity;
};

//main function
int main() {
  int num_of_snacks = 0;
  //Pointer variable declared to point to allocated heap space.
  struct snack *snack_array = NULL;

  //Asks user for the number of snacks going to be added.
  printf("Enter a number of snacks: ");
  //Scans in user input.
  scanf("%d", &num_of_snacks);

  //Call malloc to allocate the appropriate number of bytes for the array.
  snack_array = malloc(num_of_snacks * sizeof(struct snack));

  //Checks return value and handles error.
  if (snack_array == NULL) {
    printf("Error: malloc has failed.");
    exit(1);
  }

  //For loop for adding the n amount of snacks' properties.
  for (int i = 0; i < num_of_snacks; i++) {
    //Asks user for snack name and puts into snack_array.
    printf("\nEnter a name: ");
    scanf("%s", snack_array[i].name);
    //Asks user for snack cost and puts into snack_array.
    printf("Enter a cost: ");
    scanf("%f", &snack_array[i].cost);
    //Asks user for snack quantity and puts into snack_array.
    printf("Enter a quantity: ");
    scanf("%d", &snack_array[i].quantity);
  }

  printf("\nWelcome to Grace's Groovy Snack Bar! (OuO)/\n\n");

  //For loop to print out snackbar
  for (int j = 0; j < num_of_snacks; j++) {
    printf("%d) %-15s \t cost: $%.2f \t quantity: %-5d\n", j, snack_array[j].name,
      snack_array[j].cost, snack_array[j].quantity);
  }

  //Frees memory and cleans up array to null.
  free(snack_array);
  snack_array = NULL;

  return 0;
}
