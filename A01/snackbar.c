#include <stdio.h>

struct snack {
  char name[100];
  double cost;
  int quantity;
}

int main() {
  double customer_money;

  printf("Welcome to Grace's Groovy Snack Bar! \(OuO)/\n\n");
  printf("How much money do you have? ");
  scanf("%f\n\n", customer_money);
  

  return 0;
}
