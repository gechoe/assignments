#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char phrase[100];
  printf("Please enter a phrase: ");
  fgets(phrase, sizeof(phrase), stdin);
  char retString[100];

  for (int i = 0; i < strlen(phrase); i++) {
    unsigned char letter = phrase[i];
    unsigned char mask2 = 0x2;
    unsigned char mask3 = 0x4;
    mask2 = letter & mask2;
    unsigned char maskout2 = letter ^ mask2;
    mask2 = mask2 << 1;
    mask3 = letter & mask3;
    unsigned char maskout3 = letter ^ mask3;
    mask3 = mask3 >> 1;
    unsigned char mask = mask2 | mask3;
    unsigned char maskout = maskout2 & maskout3;
    unsigned char flipped = maskout ^ mask;
    retString[i] = flipped;
  }

  retString[strlen(phrase)] = '\0';
  printf("scramble: %s\n", retString);
 
  return 0;
}
