#ifndef BOARD_H
#define BOARD_H
#include <stdio.h>
#define CELL_FILLED 10
#define CELL_INVALID 11

typedef int board[9][9];

typedef struct { // one ordered pair
  int x;
  int y;
} Pair;  

typedef struct { // one ordered pair of ordered pairs; used for subgrid calculation
  Pair pairs[2];
} PairList;


void print_board(board *b); // print the contents of the board
void print_board_subgrid(board *b); // print the contents of the board with subgrid separators
void set_cell_value(board *b, int y, int x, int value); // set the value of a given cell
int get_cell_value(board *b, int y, int x); // get the value of a given cell
PairList get_cell_grid(int y, int x); // get the subgrid of a given cell; returns a list of 2 ordered pairs, the start and end coordinates
int* get_grid_values(board *b, int x, int y); // get the values of all the cells in a given subgrid
bool** get_hint_grid(board *b);

#endif
