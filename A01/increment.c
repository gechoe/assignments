#include <stdio.h>
#include <stdlib.h>

void increment(int value1, int value2, int up) {
  if (value1 >= value2) {
    up++;
    printf("%d\n", up);
  }
}

int main() {
  int val1, val2, incre;

  for (int i = 0; i < 13; i++) {
    val1 = rand();
    val2 = rand();
    increment(val1, val2, incre);
    printf("%d, val 1 %d, val2 %d\n", incre, val1, val2);
  }

  return 0;
}
