#include "solver.h"
#include "board.h"
#include <stdio.h>

bool solve_board(board *b) {
  return b;
}

int solve_cell(board *b, int x, int y) {
  int guess = 1;
  if (get_cell_value(b, x, y) != 0)
    return 10;    

  while (guess <= 9) {
    set_cell_value(b, x, y, guess);
    bool conflict = false;
    for (int i = 0; i < 9; i++) { // check for the guess in all 4 directions from the target cell; if it is found, increase the guess by one
      /* print_board_subgrid(b); */
      /* printf("cell at: (%d, %d): %d\n", x, i, get_cell_value(b, x, i)); */
      /* printf("cell at: (%d, %d): %d\n", i, y, get_cell_value(b, i, y)); */
      if (get_cell_value(b, x, i) == guess || get_cell_value(b, i, y) == guess) { 
	if ((i == x && get_cell_value(b, x, y) == guess) || (i == y && get_cell_value(b, x, y) == guess))
	  continue;
	/* printf("discrepancy found; discarding.\n"); */
	guess++;
	conflict = true;
	break;
      }
    }
    if (!conflict)
      break;
  }
  if (guess <= 9) {
    set_cell_value(b, x, y, guess);
    return guess;
  }
  return 10;
}

int check_cell(board *b, int x, int y) { // returns the value a cell
  if ((*b)[x][y] != 0) {
    return (*b)[x][y];
  } else return 0;    
}
