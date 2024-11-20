#include <stdio.h>
#include "board.h"
#include "solver.h"

board game = {
    {6, 5, 0, 2, 0, 1, 0, 7, 0},
    {0, 1, 0, 0, 6, 5, 2, 0, 3},
    {2, 4, 0, 0, 0, 7, 6, 0, 0},
    {0, 0, 7, 1, 8, 0, 0, 6, 0},
    {0, 0, 2, 0, 0, 0, 4, 0, 5},
    {8, 0, 4, 5, 3, 2, 0, 0, 0},
    {0, 2, 6, 4, 0, 0, 7, 1, 0},
    {4, 0, 1, 0, 2, 0, 0, 0, 6},
    {3, 0, 0, 0, 0, 8, 9, 0, 0}
};
board* g = &game;

bool **hint_grid;

int main() {
  hint_grid = get_hint_grid(g);
  for (int i = 0; i < 9; i++) { // rows
    if (i % 3 == 0)
      printf("-------------------------------\n");
    for (int j = 0; j < 9; j++) { // columns
      if (j == 0)
	printf("|");
      printf(" %d ", hint_grid[i][j]);
      if ((j + 1) % 3 == 0)
	printf("|");
    }
    printf("\n");
  }
  printf("-------------------------------\n");

  printf("Starting puzzle...\n\033[?25l");
  print_board_subgrid(g);
  printf("\n");
  solve_board(g, hint_grid);
  printf("\033[?25h");
}
