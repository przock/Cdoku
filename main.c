#include <stdio.h>
#include "board.h"
#include "solver.h"

board game = {
    {3, 0, 0, 0, 7, 0, 0, 0, 0},
    {7, 0, 0, 8, 0, 0, 6, 1, 0},
    {2, 9, 0, 0, 0, 0, 0, 8, 0},
    {8, 0, 0, 0, 0, 0, 0, 7, 0},
    {0, 0, 0, 9, 0, 0, 0, 3, 2},
    {0, 0, 0, 0, 4, 0, 0, 0, 0},
    {0, 8, 0, 1, 0, 0, 0, 0, 0},
    {0, 4, 0, 0, 8, 0, 7, 2, 5},
    {0, 0, 2, 0, 6, 4, 0, 0, 0}
};
board* g = &game;

int main() {
  printf("Starting puzzle...\n");
  print_board_subgrid(g);
  printf("\n");
  solve_board(g);
}
