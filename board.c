#include "board.h"
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

void print_board(board *b) { // print the contents of the board
  int i = 0;
  while (i < 81) {
    int x = i % 9; // column
    int y = i / 9; // row

    int current = get_cell_value(b, x, y);
    printf(" %d ", current);
    if (x == 8) {
      printf("\n");
    }      

    i++;
  }
}

void print_board_subgrid(board *b) { // print the contents of the board with subgrid separators
  int i = 0;
  bool newline = true;
  while (i < 81) {
    int x = i % 9; // column
    int y = i / 9; // row
    int current = get_cell_value(b, x, y);

    if (y % 3 == 0 && newline == true) {
      printf("-------------------------------\n");
      newline = false;
    }

    if (x % 3 == 0) {
      printf("|");
    }      

    printf(" %d ", current);

    if (x == 8) {
      printf("|\n");
      newline = true;
    }      

    i++;
  }
  printf("-------------------------------\n");
}

void print_board_cool(board *b, int delay) { // print the contents of the board slowly
  int i = 0;
  bool newline = true;
  while (i < 81) {
    usleep(delay);
    int x = i % 9; // column
    int y = i / 9; // row
    int current = get_cell_value(b, x, y);

    if (y % 3 == 0 && newline == true) {
      printf("-------------------------------\n");
      newline = false;
    }

    if (x % 3 == 0) {
      printf("|");
    }      

    printf(" %d ", current);

    if (x == 8) {
      printf("|\n");
      newline = true;
    }      

    i++;
  }
  printf("-------------------------------\n");
}

void set_cell_value(board *b, int y, int x, int value) {
  (*b)[x][y] = value;
}  

int get_cell_value(board *b, int y, int x) {
  return (*b)[x][y];
}

int get_cell_value_filled(board *b, int y, int x) {
  return (*b)[x][y] ? 1 : 0;
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

int** get_hint_board(board *b) { // get a 2d array of the pre-filled squares so we know which ones to skip when backtracking
    int** arr = (int**)malloc(9 * sizeof(int*));
    if (!arr) {
      printf("Memory error from get_hint_board!");
      exit(1);
    }      

    for (int i = 0; i < 9; i++) {
        arr[i] = (int*)malloc(9 * sizeof(int));
    }

    // iterate though the entire board and build a bool[][] of whether or not cells are filled

    int i = 0;
    while (i < 81) {
      int x = i % 9; // column
      int y = i / 9; // row
      
      arr[x][y] = get_cell_value_filled(b, x, y);
      i++;
    }
    
    return arr;
}

int** board_from_json_cartesian(cJSON* json) {
  int** arr = (int**)malloc(9 * sizeof(int*));
  if (!arr) {
    printf("Memory error from set_board_from_json_cartesian!");
    exit(1);
  }      
  
  for (int i = 0; i < 9; i++) {
    arr[i] = (int*)malloc(9 * sizeof(int));
  }
  
  int i = 0;
  while (i < 81) {
    int x = i % 9; // column
    int y = i / 9; // row
    
    // obtain the value from the json table and push it to the board
    char coords[20];
    sprintf(coords, "%d,%d", x+1, y+1);
    cJSON *value = cJSON_GetObjectItem(json, coords);
    if (cJSON_IsNumber(value)) {
      arr[x][y] = value->valueint;
    }
    i++;
  }
  
  return arr;
}
