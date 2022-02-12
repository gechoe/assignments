//---------------------------------------------------------------------
// magic_square.c
// CS223 - Spring 2022
// Identify whether a matrix is a magic square
// Name: Grace Choe
// Date: 2/9/2022
//
#include <stdio.h>
#include <stdlib.h>

//is_magic_square function
//Checks if the the "squre" is a magic square using the array and column inputs.
void is_magic_square(int** array, int column) {
  int diag_one_total = 0;
  int diag_two_total = 0;
  int row_sum = 0;
  int column_sum = 0;

  //for loop finds the two diagonal totals.
  for (int m = 0; m < column; m++) {
    diag_one_total += array[m][m];
    diag_two_total += array[m][column - 1 - m];
  }

  //If diagonals are not equal, it is not a square:
  if (diag_one_total != diag_two_total) {
    printf("M is NOT a magic square!\n");
    return;
  } else {
    //for loop Finds the sum of every row and column and sees if they are equal
    //to each other, if not equal prints out not a magic square.
    for (int n = 0; n < column; n++) {
      row_sum = 0;
      column_sum = 0;
      if (row_sum == column_sum) {
        for (int p = 0; p < column; p++) {
          row_sum += array[n][p];
          column_sum += array[p][n];
        }
      } else {
        printf("M is NOT a magic square!\n");
        return;
      }
    }
  }

  /**
   * Since we know the diagonals' sums equal each other and the rows' sum and
   * columns' sum equals each other, we check if (one from the pair) row equals
   * a diagonal.
   */
  if (diag_two_total == row_sum) {
    printf("M is a magic square (magic constant = %d)\n", row_sum);
  } else {
    printf("M is NOT a magic square!\n");
    return;
  }
}

//main function
int main() {
  int row = 0;
  int col = 0;
  int val = 0;
  //a pointer to an int pointer since 2d array declaration.
  int **two_d_array = NULL;

  //scans in the first two values (the dimensions of the matrix
  scanf(" %d", &row);
  scanf(" %d", &col);

  //Allocates an array of [row amount] pointers to ints
  two_d_array = malloc(sizeof(int *) * row);

  //For each row, malloc space is created for its column elements, added to array
  // to make array of arrays (2d array).
  for (int i = 0; i < row; i++) {
    two_d_array[i] = malloc(sizeof(int) * col);
  }

  //Checks return value and handles error.
  if (two_d_array == NULL) {
    printf("ERROR: malloc failed!\n");
    exit(1);
  }

  //Inputs each value of given magic square into its appropriate spot in 2d array
  //Value is also printed out appropriately.
  for (int j = 0; j < row; j++) {
    for (int k = 0; k < col; k++) {
      scanf("%d", &val);
      two_d_array[j][k] = val;

      printf("%-3d", val);
    }
    printf("\n");
  }

  //Checks if magic_square.
  is_magic_square(two_d_array, col);

  //Frees the malloc'd space within the array (the column elements).
  for (int i = 0; i < row; i++) {
    free(two_d_array[i]);
    two_d_array[i] = NULL;
  }

  //Frees the outer array, thus freeing the entire "2d array".
  free(two_d_array);
  two_d_array = NULL;

  return 0;
}
