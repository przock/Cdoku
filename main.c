#include <stdio.h>
#include "board.h"
#include "solver.h"

board game = {
    {2, 3, 0, 9, 4, 0, 0, 5, 0},
    {8, 0, 0, 5, 3, 2, 1, 0, 9},
    {9, 5, 0, 1, 0, 0, 7, 0, 3},
    {0, 8, 7, 0, 0, 0, 6, 3, 0},
    {4, 0, 3, 0, 7, 1, 0, 8, 0},
    {0, 0, 2, 0, 5, 3, 0, 0, 0},
    {3, 6, 8, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 4, 1, 9, 0, 0, 0},
    {0, 0, 9, 0, 0, 8, 0, 7, 2}
};

board* g = &game;
int** hint_board;

int main() {
  hint_board = get_hint_board(g);

  printf("Starting puzzle...\n\n\033[?25l");
  print_board_subgrid(g);
  printf("\n");
  solve_board(g, hint_board);
  printf("\033[?25h");
}
