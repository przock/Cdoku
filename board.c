#include "board.h"

void print_board(board *b) { // print the contents of the board
  for (int i = 0; i < 9; i++) { // rows
    for (int j = 0; j < 9; j++) { // columns
      printf("(%d)", (*b)[i][j]);
    }
    printf("\n");
  }
}

void print_board_subgrid(board *b) { // print the contents of the board with subgrid separators
  for (int i = 0; i < 9; i++) { // rows

    if (i % 3 == 0)
      printf("-------------------------------\n");

    for (int j = 0; j < 9; j++) { // columns

      if (j == 0)
	printf("|");

      printf(" %d ", (*b)[i][j]);

      if ((j + 1) % 3 == 0)
	printf("|");

    }
    printf("\n");
  }
  printf("-------------------------------\n");
}

void set_cell_value(board *b, int y, int x, int value) {
  (*b)[x][y] = value;
}  

int get_cell_value(board *b, int y, int x) {
  return (*b)[x][y];
}  
