//---------------------------------------------------------------------
// match_braces.c 
// CS223 - Spring 2022
// Identify matched braces from a given file
// Name: Grace Choe
// Date: 2/11/2022
//
#include <stdio.h>
#include <stdlib.h>

//node struct type defined with sym, linenum, colnum, and pointer next.
struct node {
  char sym;
  int linenum;
  int colnum;
  struct node* next;
};

// Push a new node to a stack (implemented as a linked list).
// The new node should store the given symbol, line number, and column number
// Param sym: a character symbol, '{' for this program
// Param line: the line number of the symbol
// Param line: the column number of the symbol
// Param top: the top node of the stack (NULL if empty)
// Returns the new top of the stack
struct node* push(char sym, int line, int col, struct node* top) {
  //Pointer variable declared to point to allocated heap space.
  struct node* new_node = NULL;

  //Malloc called to allocate the appropriate space.
  new_node = malloc(sizeof(struct node));

  //Checks return value and handles error.
  if (new_node == NULL) {
    printf("ERROR malloc failed!\n");
    exit(1);
  }

  //Gives new_node its properties based on function param.
  new_node->sym = sym;
  new_node->linenum = line;
  new_node->colnum = col;
  new_node->next = NULL;

  //If top is not null then new_node points to top as new_node is the new "top".
  if (top != NULL) {
    new_node->next = top;
  }
  //If top is null, new_node is added to the top of the "stack"
  //AND also functions to reassign "top" to be new_node even if top is not null.
  top = new_node;

  return top;
}

// Pop the top node from a stack (implemented as a linked list) and frees it
// Param top: the top node of the current stack (NULL if empty)
// Returns the new top of the stack
struct node* pop(struct node* top) {
  struct node* node = NULL;
  //if else statement to pop out top node, and adjust pointers accordingly.
  if (top == NULL) {
    printf("Nothing to be popped!");
  } else {
    node = top->next;

    free(top);
    top = node;
  }

  return top;
}

// Delete (e.g. free) all nodes in the given stack
// Param top: the top node of the stack (NULL if empty)
void clear(struct node* top) {
  struct node* node = NULL;

  //while loop to clear everything in the "stack"
  //frees from the top until everything is free'd and nulled.
  while (top != NULL) {
    node = top->next;

    free(top);
    top = node;
  }
  top = NULL;

  free(node);
  node = NULL;
}

// Print all nodes in the given stack (from top to bottom)
// Param top: the top node of the stack (NULL if empty)
void print(struct node* top) {
  while (top != NULL) {
    //Going from the top of the "Stack" prints the first node then goes down to
    //print all the other nodes one by one.
    printf("%c  %d  %d\n", top->sym, top->linenum, top->colnum);
    top = top->next;
  }
}

//main function
int main(int argc, char* argv[]) {
  //Declares a FILE * variable.
  FILE *file;
  int line = 1;
  int column = 0;
  int char_val;
  //Pointer variable to point to the top/head of the stack.
  struct node* head = NULL;

  //Open the file based on commandline input.
  if (argv != NULL) {
    file = fopen(argv[1], "r");
  }

  //If the commandline input is less then 2 (does not include file name) or
  //greater than 2 (has more than just the program name and filename) then
  //the usage with the program run command is printed out.
  if (argc < 2 || argc > 2) {
    printf("usage: %s\n", argv[0]);
    exit(1);
  }

  //Handles if file is null.
  if (file == NULL) {
    printf("Cannot open file:  %s\n", argv[1]);
    exit(1);
  }

  //while loop for reading until no more can be read of the file, reading until
  //the end of the file (EOF).
  while ((char_val = fgetc(file)) != EOF) {
    column++; //Adds + 1 to column everytime a new char_val is read.
    if (char_val == '\n') {
      line++; //line incrementor for every new line.
      column = 0; //Resets column every new line.
    }

    //If statement for pushing a new node to the "stack" if { is encountered.
    if (char_val == '{') {
      head = push(char_val, line, column, head); //pushes
    }

    //If statement for stating if a matching brace was found or not found based
    //on }'s presence.
    if (char_val == '}') {
      if (head != NULL) {
        printf("Founding matching braces: (%d, %d) -> (%d, %d)\n",
          head->linenum, head->colnum, line, column);
        head = pop(head);
      } else {
        printf("Unmatched brace on Line %d and Column %d\n", line, column);
      }
    }
  }

  //If head is not null by the end, that means there are { leftover, so they are
  //unmatched, and are then popped and printed out until head (stack) is empty.
  while (head != NULL) {
    printf("Unmatched brace on Line %d and Column %d\n", head->linenum,
      head->colnum);
    head = pop(head);
  }

  //Closes the file.
  fclose(file);

  //Doesn't occur since everything is popped to find unmatched braces,
  //but added in code to clear just in case.
  if (head != NULL) {
    clear(head);
  }
  return 0;
}
