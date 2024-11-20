#include "solver.h"
#include "board.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

bool solve_board(board *b, int **hint) {
  int i = 0;
  bool backtrack = false;
  while (i < 81) {
    usleep(100000);
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
	if (x == 0 && y == 0) {printf("No valid solution!\n"); exit(EXIT_FAILURE);}
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

/* bool solve_board(board *b, bool** hint) { */
/*   int i = 0; */
/*   while (i < 81) { */
/*     int x = i % 9; // column */
/*     int y = i / 9; // row */
    
/*     bool backtrack = false; */
/*     int min_guess = 1; */
/*     int current = get_cell_value(b, x, y); */
/*     printf("(%d, %d): %d\n", x, y, current); */
    
/*     if (hint[x][y] == true) { */
/*       if (backtrack) { */
/* 	i--; */
/* 	i--; */
/* 	print_board_subgrid(b); */
/* 	continue; */
/*       } */
/*     } */
    
/*     usleep(500000); */
/*     if (backtrack) { */
/*       min_guess = get_cell_value(b, x, y) + 1; */
/*       set_cell_value(b, x, y, 0); */
/*     } */
/*     if (!backtrack) */
/*       min_guess = 1; */
    
/*     int guess = check_cell(b, i, j, min_guess); */
/*     backtrack = false; */
/*     /\* printf("guess for (%d, %d): %d\n", i, j, guess); *\/ */
/*     /\* printf("min: %d\n", min_guess); *\/ */
    
/*     if (guess != 10) { */
/*       if (guess != 11) { */
	
/* 	set_cell_value(b, i, j, guess); */
/* 	/\* printf("set (%d, %d) to %d\n", i, j , guess); *\/ */
/* 	print_board_subgrid(b); */
/* 	/\* printf("\033[13A"); *\/ */
/*       } else if (guess == 11) { */
/* 	if (i == 0) { */
/* 	  j--; */
/* 	  j--; */
/* 	  i = 7; */
/* 	  /\* min_guess++; *\/ */
/* 	  backtrack = true; */
/* 	} else { */
/* 	  i--; */
/* 	  i--; */
/* 	  /\* min_guess++; *\/ */
/* 	  backtrack = true; */
/* 	} */
/*       } */
/*     } */
/*     /\* printf("\033[14A"); *\/ */
/*     /\* print_board_subgrid(b); *\/ */
/*     return true; */
/*   } */
/* } */

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

int check_cell(board *b, int x, int y, int min_guess) { // returns something...
  PairList grid_bounds = get_cell_grid(x, y);
  int guess = min_guess;
  if (get_cell_value(b, x, y) != 0)
    return CELL_FILLED;

  while (guess <= 9) {
    /* printf("conflict is now false\n"); */
    bool conflict = false;
    bool lateral = false;
    bool pass = false;

    for (int i = 0; i < 9; i++) { // check for the guess in all 4 directions from the target cell; if it is found, increase the guess by one

      /* if ((given.x == current_row.x && given.y == current_row.y) || (given.x == current_column.x && given.y == current_column.y)) { */
      /* 	printf("this should be the same: xy(%d, %d), iy(%d, %d)", x, y, i, y); */
      /* 	continue; */
      /* } */


      /* printf("i: %d\n", i); */
      /* printf("lat: cell at: (%d, %d): %d\n", x, i, get_cell_value(b, x, i)); */
      /* printf("lat: cell at: (%d, %d): %d\n", i, y, get_cell_value(b, i, y)); */
      /* print_board_subgrid(b); */

      if (i == 8) {
        /* printf("pass is now true\n"); */
        pass = true;
      }

      if (get_cell_value(b, x, i) == guess || get_cell_value(b, i, y) == guess) {

	/* if ((i == x && get_cell_value(b, x, y) == guess) || (i == y && get_cell_value(b, x, y) == guess)) { // problematic */
	/*   printf("failsafe\n"); */
	/*   continue; */
	/* } */

	/* printf("discrepancy found; discarding.\n"); */
	conflict = true;
	guess++;
	break;
      }

      if (!conflict && pass) {
	/* printf("lateral checks passed, performing subgrid checks"); */
	lateral = true;
      }

      /* printf("%s\n", conflict ? "true" : "false"); */

      if (!lateral) { /* printf("skipping subgrid check\n"); */ continue; }
      for (int j = grid_bounds.pairs[0].x; j <= grid_bounds.pairs[1].x; j++) {
	for (int k = grid_bounds.pairs[0].y; k <= grid_bounds.pairs[1].y; k++) {
	  /* printf("sub: cell at: (%d, %d): %d\n", j, k, get_cell_value(b, j, k)); */
	  if (get_cell_value(b, j, k) == guess) {
	    
	    if ((j == x && get_cell_value(b, x, y) == guess) || (k == y && get_cell_value(b, x, y) == guess))
	      continue;

	    /* printf("discrepancy found in subgrid; back to lateral checks.\n"); */
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
      /* print_board_subgrid(b); */
      return guess;
    }
  }
    return CELL_INVALID;
}
