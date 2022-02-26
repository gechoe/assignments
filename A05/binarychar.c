/**
 * binarychar.c
 * Author: Grace Choe
 * Date: 2/24/2022
 *
 * Description:
 * This program implements a program tthat asks the user for an 8-bit binary
 * number and outputs the corresponding ASCII character (binary to ASCII),
 * assuming that the user enters a valid 8-bit binary number).
 */

#include <stdio.h>

//power function
//Calculates the power with the values: base and exponent.
//Can use math.h library's pow() by editing the makefile with -lm, but decided
//to code power instead.
int power(int base, int exponent) {
  int answer = 1;

  //for loop, loops through multiplying answer to its base exponent # of times.
  for (int i = 0; i < exponent; i++) {
    answer =  answer * base;
  }

  return answer;
}

//bintodec function
//Gets the decimal equivalent of the binary number.
int bintodec(int bin) {
  int dec = 0, remainder, pow_val, i = 0;
 
  //while loop, calculates the total decimal value.
  while (bin != 0) {
    remainder = bin % 10; //Gets the last binary digit.
    bin /= 10; //Removes the last binary digit.
    //Finds the decimal value of the last binary digit using power function.
    pow_val = remainder * power(2, i);
    dec += pow_val; //Calculates the decimal value total.
    i++; //Increments to get the next power of 2.
  }
  
  return dec;
}

//main function
//Asks user for 8-digit binary number input, uses bintodec to convert it into
//decimal. Finds ascii equivalent of the decimal number.
int main() {
  int binary, decimal;

  printf("Enter 8 bits: ");
  scanf("%d", &binary);

  decimal = bintodec(binary);
  //Since %c and %d are interchangeable within the ascii, printing out the %c
  //version of int decimal gives decimal's ascii char equivalent.
  printf("Your character is: %c\n", decimal);
  
  return 0;
}
