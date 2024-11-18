#ifndef SOLVER_H
#define SOLVER_H
#include <stdbool.h>
#include "board.h"

bool solve_board(board *b); // begin the solving algorithm on a game board
int solve_cell(board *b, int y, int x); // solve a single cell in a board; used in solve_board; returns either 10 for failure or non-10 for a guess
int check_cell(board *b, int y, int x); // returns the value of a given cell

#endif
