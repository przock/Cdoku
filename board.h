#ifndef BOARD_H
#define BOARD_H
#include <stdio.h>
#define CELL_FILLED 10

typedef int board[9][9];

void print_board(board *b); // print the contents of the board
void print_board_subgrid(board *b); // print the contents of the board with subgrid separators
void set_cell_value(board *b, int y, int x, int value); // set the value of a given cell
int get_cell_value(board *b, int y, int x); // get the value of a given cell
int get_cell_grid(board *b, int y, int x); // get the subgrid of a given cell; returns the CEM subgrid index

#endif
