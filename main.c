#include <stdio.h>
#include "board.h"
#include "solver.h"
#include <stdlib.h>
#include <signal.h>

board game = {
    {6, 3, 0, 0, 0, 0, 0, 8, 1},
    {0, 2, 0, 0, 0, 3, 0, 0, 0},
    {0, 0, 0, 0, 1, 7, 4, 3, 0},
    {0, 9, 0, 4, 0, 0, 5, 7, 0},
    {0, 0, 0, 7, 6, 2, 0, 0, 0},
    {0, 8, 0, 0, 0, 0, 6, 0, 0},
    {0, 6, 0, 0, 2, 0, 0, 0, 0},
    {3, 0, 9, 0, 0, 0, 0, 6, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 9}
};

board* g = &game;
int** hint_board;
bool solved;

void handle_signal(int signal) {
  if (signal == SIGINT) {
    print_board_subgrid(g);
    printf("\nInterrupt! Exiting...\n");
    printf("\033[?25h");
    exit(0);
  }
}

int main() {

  signal(SIGINT, handle_signal);

  hint_board = get_hint_board(g);
  printf("Starting puzzle...\n\033[?25l");
  print_board_subgrid(g);
  printf("\n");  
  solved = solve_board(g, hint_board, 200);
  /* solved = solve_board_quick(g, hint_board); */
  /* print_board_subgrid(g); */
  if (!solved) {printf("No solution!\n"); exit(EXIT_FAILURE);}
  printf("Solved!\n");
  printf("\033[?25h");
}
