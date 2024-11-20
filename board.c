#include "board.h"
#include <stdlib.h>
#include <stdbool.h>

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

bool get_cell_value_filled(board *b, int y, int x) {
  return (*b)[x][y] ? true : false;
}

PairList get_cell_grid(int y, int x) {
  Pair start;
  Pair end;
  Pair subgrid;

  subgrid.x = y / 3;
  subgrid.y = x / 3;
  /* printf("(%d, %d)\n", subgrid.x, subgrid.y); debug */

  start.x = subgrid.x * 3;
  start.y = subgrid.y * 3;
  end.x = (subgrid.x * 3) + 2;
  end.y = (subgrid.y * 3) + 2;

  PairList bounds = {{start, end}};
  return bounds;
}

int* get_grid_values(board *b, int x, int y) {
  int* values = malloc(9 * sizeof(int));
  if (!values) {
    printf("memory error from get_grid_value");
    exit(1);
  }

  int i = 0;
  int v;
  PairList grid_bounds = get_cell_grid(x, y);

  for (int j = grid_bounds.pairs[0].x; j <= grid_bounds.pairs[1].x; j++) {
    for (int k = grid_bounds.pairs[0].y; k <= grid_bounds.pairs[1].y; k++) {
      /* printf("(%d, %d): ", j, k); */
      /* printf("%d\n", get_cell_value(b, j, k)); */
      v = get_cell_value(b, j, k);
      values[i] = v;
      /* printf("setting %d to %d", i, v); */
      i++;
    }
  }
  return values;
}

bool** get_hint_grid(board *b) { // get a 2d array of the pre-filled squares so we know which ones to skip when backtracking
    bool** arr = (bool**)malloc(9 * sizeof(int*));
    if (!arr) {
      printf("Memory error from get_hint_grid!");
      exit(1);
    }      

    for (int i = 0; i < 9; i++) {
        arr[i] = (bool*)malloc(9 * sizeof(int));
    }

    // iterate though the entire board and build a bool[][] of whether or not cells are filled
    for (int i = 0; i < 9; i++) { // rows
      for (int j = 0; j < 9; j++) { // columns
	arr[j][i] = get_cell_value_filled(b, i, j);
      }
    }

    return arr;
}
