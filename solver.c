#include "solver.h"
#include "board.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

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

    if (backtrack && !hint[x][y]) {
      set_cell_value(b, x, y, 0);
    }

    int min_guess = backtrack ? current + 1 : 1;
    int valid_choice = check_cell(b, x, y, min_guess);
    int guess = hint[x][y] ? CELL_FILLED : valid_choice;

    /* printf("guess for (%d, %d): %d\n", x, y, guess); */

    if (guess == CELL_FILLED) { // 10

      if (backtrack) {
	if (x == 0 && y == 0) {return false;}
        i--;
        continue;
      }

      print_board_subgrid(b);
      printf("\033[13A");
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
    }

    print_board_subgrid(b);
    printf("\033[13A");
    /* printf("(%d, %d): %d\n", x, y, current); */
    
    if (!backtrack) {i++;}
  }

  printf("\033[14A");
  print_board_subgrid(b);
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

int solve_cell(board *b, int x, int y, int min_guess) {
  PairList grid_bounds = get_cell_grid(x, y);
  int guess = min_guess;
  if (get_cell_value(b, x, y) != 0)
    return CELL_FILLED;

  while (guess <= 9) {
    set_cell_value(b, x, y, guess);
    printf("conflict is now false\n");
    bool conflict = false;
    bool lateral = false;
    bool pass = false;

    for (int i = 0; i < 9; i++) { // check for the guess in all 4 directions from the target cell; if it is found, increase the guess by one
      Pair given = {x, y};
      Pair current_row = {x, i};
      Pair current_column = {i, y};

      if ((given.x == current_row.x && given.y == current_row.y) || (given.x == current_column.x && given.y == current_column.y)) {
	printf("this should be the same: xy(%d, %d), iy(%d, %d)", x, y, i, y);
	continue;
      }

      printf("i: %d\n", i);
      printf("lat: cell at: (%d, %d): %d\n", x, i, get_cell_value(b, x, i));
      printf("lat: cell at: (%d, %d): %d\n", i, y, get_cell_value(b, i, y));
      print_board_subgrid(b);

      if (i == 8) {
        printf("pass is now true\n");
        pass = true;
      }

      if (get_cell_value(b, x, i) == guess || get_cell_value(b, i, y) == guess) {

	printf("discrepancy found; discarding.\n");
	conflict = true;
	guess++;
	break;
      }

      if (!conflict && pass) {
	lateral = true;
      }

      printf("%s\n", conflict ? "true" : "false");

      if (!lateral) { printf("skipping subgrid check\n"); continue; }
      for (int j = grid_bounds.pairs[0].x; j <= grid_bounds.pairs[1].x; j++) {
	for (int k = grid_bounds.pairs[0].y; k <= grid_bounds.pairs[1].y; k++) {
	  printf("sub: cell at: (%d, %d): %d\n", j, k, get_cell_value(b, j, k));
	  if (get_cell_value(b, j, k) == guess) {
	    
	    if ((j == x && get_cell_value(b, x, y) == guess) || (k == y && get_cell_value(b, x, y) == guess))
	      continue;

	    printf("discrepancy found; discarding.\n");
	    conflict = true;
	    break;
	  }
	}
	if (conflict) break;
      }
    }

    if (conflict && pass) {
      guess++;
      continue;
    } else if (conflict && !pass) { continue; }
    if (guess <= 9) {
      set_cell_value(b, x, y, guess);
      print_board_subgrid(b);
      return guess;
    }
  }
  return CELL_INVALID;
}

int check_cell(board *b, int x, int y, int min_guess) {
    PairList grid_bounds = get_cell_grid(x, y);
    int guess = min_guess;

    if (get_cell_value(b, x, y) != 0)
        return CELL_FILLED; // 10

    while (guess <= 9) {
        bool conflict = false;

        for (int i = 0; i < 9; i++) { // check each row and column
            if (get_cell_value(b, x, i) == guess || get_cell_value(b, i, y) == guess) {
                conflict = true;
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
                        break;
                    }
                }
                if (conflict) break;
            }
        }

        if (!conflict) {
            return guess; // valid guess
        }

        guess++; // try again
    }

    return CELL_INVALID; // no valid guess
}
