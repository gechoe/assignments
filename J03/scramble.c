#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char phrase[100];
  printf("Please enter a phrase: ");
  scanf("%s", phrase);
  char retString[100];

  for (int i = 0; i < strlen(phrase); i++) {
    unsigned int letter = phrase[i];
    unsigned int mask1 = 0x0F00;
    unsigned int mask2 = 0x00F0;
    unsigned int mask3 = 0xF00F;
    unsigned int first = letter & mask1;
    unsigned int second = letter & mask2;
    unsigned int third = letter & mask3;
    unsigned int shiftl = second >> 1;
    unsigned int shiftr = first << 1;
    unsigned flipped = (shiftr | shiftl) | third;

    retString[i] = flipped;
  }

  printf("scramble: %s\n", retString);

  return 0;
}
