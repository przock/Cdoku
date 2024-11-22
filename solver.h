#ifndef SOLVER_H
#define SOLVER_H
#include <stdbool.h>
#include "board.h"

bool solve_board(board *b, int** hint, int delay); // begin the solving algorithm on a game board
bool solve_board_quick(board *b, int** hint); // begin the solving algorithm on a game board without printing

int check_cell(board *b, int y, int x, int min_guess); // returns the value of a given cell

#endif
