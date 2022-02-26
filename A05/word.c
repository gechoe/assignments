/**
 * word.c
 * Author: Grace Choe
 * Date: 2/23/2022
 *
 * Description:
 * This program asks the user for a four letter word and stores the characters
 * within a single unsigned int, assuming that the user enters a valid four
 * letter word. Debugs are also printed out and kept like the example output
 * given. The debugs prints the hexadecimal values of each character, shifted to
 * its correct spot.
 */

#include <stdio.h>
#include <string.h>

//main function
int main() {
  //string char array to hold the four letter word user input.
  char string[4];
  //Holds the char version of each letter's hex value.
  char hex[9];
  //num holds the hex formatted version of each letter's hex value.
  //shifting holds the correctly shifter version of each letter's hex value.
  unsigned int num, shifting;
  unsigned int hex_add = 0x00000000;
  int shift = 0;

  printf("Enter 4 characters: ");
  scanf(" %s", string);

  for (int k = 3; k >= 0; k--) {
    //Reformats the specified letter of string into hex format.
    sprintf(hex, "000000%x", string[k]);
    //Transfers hex into %08X formatted unsigned int num.
    sscanf(hex, "%08X", &num);
    
    //Shifts the hex value to its correctly shifted spot.
    shifting  = num << shift;
    shift += 8;
    
    //Adds the shifted value to hex_add which holds the final single unsigned
    //int for the word's hex value.
    hex_add += shifting;
    printf("Debug: %c = 0x%08X\n", string[k], shifting);
  }

  printf("Your number is: %d (0x%08X)\n", hex_add, hex_add);

  return 0;
}
