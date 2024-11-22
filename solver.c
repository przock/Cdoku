#include "solver.h"
#include "board.h"
#include "flags.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

bool solve_board(board *b, int **hint, int delay) {
  int i = 0;
  bool backtrack = false;
  while (i < 81) {
    usleep(delay);
    /* if (i < 0) { */
    /*   printf("No valid solution!\n"); */
    /*   exit(EXIT_FAILURE); */
    /* } */

    int x = i % 9; // column
    int y = i / 9; // row
    
    int current = get_cell_value(b, x, y);

    if (backtrack && !hint[x][y]) {set_cell_value(b, x, y, 0);}

    int min_guess = backtrack ? current + 1 : 1;
    int valid_choice = check_cell(b, x, y, min_guess);
    int guess = hint[x][y] ? CELL_FILLED : valid_choice;

    if (guess == CELL_FILLED) { // 10

      if (backtrack) {
	if (x == 0 && y == 0) {return false;}
        i--;
        continue;
      }

      print_board_subgrid(b);
      if (!verbose) {
        printf("\033[13A");
      }
      i++;
      continue;
    }

    if (guess != CELL_INVALID) { // 11
      set_cell_value(b, x, y, guess);
      backtrack = false;

    } else {
      /* printf("discrepancy at (%d, %d)\n", x, y); */

      backtrack = true;
      i--;

      if (verbose) print_board_subgrid(b);
    }

    print_board_subgrid(b);
    if (!verbose) {printf("\033[13A");} else {printf("(%d, %d): %d\n", x, y, current);} 
    
    if (!backtrack) {i++;}
  }

  if (!verbose) {
    printf("\033[14A");
    print_board_subgrid(b);
  }
  return true;
}

bool solve_board_quick(board *b, int **hint) {
  int i = 0;
  bool backtrack = false;
  while (i < 81) {

    int x = i % 9; // column
    int y = i / 9; // row
    
    int current = get_cell_value(b, x, y);

    if (backtrack && !hint[x][y]) {
      set_cell_value(b, x, y, 0);
    }

    int min_guess = backtrack ? current + 1 : 1;
    int valid_choice = check_cell(b, x, y, min_guess);
    int guess = hint[x][y] ? CELL_FILLED : valid_choice;

    if (guess == CELL_FILLED) { // 10

      if (backtrack) {
	if (x == 0 && y == 0) {return false;}
        i--;
        continue;
      }

      i++;
      continue;
    }

    if (guess != CELL_INVALID) { // 11
      set_cell_value(b, x, y, guess);
      backtrack = false;

    } else {

      backtrack = true;
      i--;
    }

    if (!backtrack) {i++;}
  }

  return true;
}

int check_cell(board *b, int x, int y, int min_guess) {
    PairList grid_bounds = get_cell_grid(x, y);
    int guess = min_guess;

    if (verbose) {printf("Checking cell (%d, %d)\n", x, y);}

    if (get_cell_value(b, x, y) != 0)
      return CELL_FILLED; // 10
    
    while (guess <= 9) {
      bool conflict = false;
      
      for (int i = 0; i < 9; i++) { // check each row and column
	if (get_cell_value(b, x, i) == guess || get_cell_value(b, i, y) == guess) {
	  conflict = true;
	  if (verbose) {printf("Lateral discrepancy at (%d, %d)\n", x, y);}
	  break;
	}
      }
      
      if (!conflict) {
	// subgrids
	for (int j = grid_bounds.pairs[0].x; j <= grid_bounds.pairs[1].x; j++) {
	  for (int k = grid_bounds.pairs[0].y; k <= grid_bounds.pairs[1].y; k++) {
	    
	    if (j == x && k == y) continue; // skip the one we're checking
	    
	    if (get_cell_value(b, j, k) == guess) {
	      conflict = true;
	      if (verbose) {printf("Subgrid discrepancy at (%d, %d)\n", j, k);}
	      break;
	    }
	  }
	  if (conflict) break;
	}
      }
      
      if (!conflict) {
	if (verbose) {printf("Valid guess found: %d\n", guess);}
	return guess; // valid guess
      }
      
      guess++; // try again
      if (verbose) {printf("No valid guess yet, try again (New guess: %d)\n", guess);}
    }
    
    if (verbose) {printf("No valid guess for (%d, %d)\n", x, y);}
    return CELL_INVALID; // no valid guess
}
