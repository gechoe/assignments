/**
 * snackbar.c
 * Author: Grace Choe
 * Date: 1/27/2022
 * 
 * Description:
 * This program welcomes the user to a snackbar. They ask the user to
 * input the amount of money they have and then presents the user with
 * a list of available snacks to purchase. The user then inputs the snack
 * they would like and is given an output according to the snack's cost,
 * quantity, etc.
 */

#include <stdio.h>
#include <string.h>

//snack struct type defined with name, cost, quantity.
struct snack {
  char name[100];
  double cost;
  int quantity;
};

//main function 
int main() {
  double customer_money;
  int chosen_snack;
  double change;

  //Welcomes user, then asks for user input of how much money they have.
  printf("Welcome to Grace's Groovy Snack Bar! \(OuO)/\n\n");
  printf("How much money do you have? ");
  //Scans in user input.
  scanf("%lg", &customer_money);
  printf("\n");

  //Array created to store four snacks.
  struct snack snack_types[4];

  //Accessing and assigning values into the array for the four snacks.
  strcpy(snack_types[0].name, "Haribo Gummy Bears");
  snack_types[0].cost = 1.50;
  snack_types[0].quantity = 11;

  strcpy(snack_types[1].name, "Pringles");
  snack_types[1].cost = 2.99;
  snack_types[1].quantity = 6;

  strcpy(snack_types[2].name, "Caviar with Crackers");
  snack_types[2].cost = 50.75;
  snack_types[2].quantity = 3;

  strcpy(snack_types[3].name, "Unicorn Magic Chips");
  snack_types[3].cost = 0.99;
  snack_types[3].quantity = 0;

  //for loop to print out and list all four snacks and their according information.
  for (int i = 0; i < 4; i++) {
    printf("%d) %-20s \t cost: $%.2f \t quantity: %-5d\n", i, snack_types[i].name,
     snack_types[i].cost, snack_types[i].quantity);
  } 

  //Asks user which snack they would like and scans in their input.
  printf("\nWhat snack would you like to buy? [0,1,2,3] ");
  scanf("%d", &chosen_snack);

  //Responses to user's input.
  if ((chosen_snack > 3) || (chosen_snack < 0)) {
    //Invalid snack option chosen, prints out:
    printf("Invalid option. Please choose a snack from the given menu next time.\n\n");
  } else {
    if (snack_types[chosen_snack].quantity <= 0) {
      //Snack is out of stock, prints out:
      printf("Sorry, we are out of %s\n\n", snack_types[chosen_snack].name);
    } else {
      if (customer_money < snack_types[chosen_snack].cost) {
        //Snack is worth more than customer has, prints out:
	printf("You can't afford it!\n\n");
      } else {
	//Customer is able to buy snack, prints out change:
        change = customer_money - snack_types[chosen_snack].cost;
        printf("You bought %s\n", snack_types[chosen_snack].name);
        printf("You have $%.2f left\n", change);
	printf("Thank you for buying, come back again! ~(OuO)~\n\n");
      }
    }
  }

  return 0;
}
