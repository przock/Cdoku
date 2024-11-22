#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "board.h"
#include "solver.h"
#include "flags.h"

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
int delay = 200;

bool solved = false;
bool verbose = false;
bool quick = false;
bool custom_delay = false;

void handle_signal(int signal) {
  if (signal == SIGINT) {
    print_board_subgrid(g);
    printf("\nInterrupt! Exiting...\n");
    printf("\033[?25h");
    exit(0);
  }
}

int main(int argc, char *argv[]) {
    int opt;
    
    while ((opt = getopt(argc, argv, OPTION_STRING)) != -1) {
        switch (opt) {
            case FLAG_JSON_CEM:
                printf("Option -j with value: %s\n", optarg);
                break;
            case FLAG_DELAY:
		custom_delay = true;
		delay = atoi(optarg);
                break;
            case FLAG_JSON:
                printf("Option -J with value: %s\n", optarg);
                break;
            case FLAG_VERBOSE:
                printf("Verbose Mode enabled\n");
		verbose = true;
		break;
            case FLAG_QUICK:
                printf("Quick Mode enabled\n");
		quick = true;
		break;
            case FLAG_HELP:
                printf("%s", HELP_STRING);
		exit(0);
            case '?':
                if (optopt == FLAG_JSON || optopt == FLAG_JSON_CEM) {
		    exit(1);
                } else {
                    fprintf(stderr, "No such flag: -%c\n", optopt);
		    exit(1);
                }
                break;
            default:
                fprintf(stderr, "Unknown error\n");
		exit(1);
        }
    }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  if (verbose) {printf("Custom delay: %s\nDelay: %d\n", custom_delay ? "true" : "false", delay);}
  
  signal(SIGINT, handle_signal);

  hint_board = get_hint_board(g);
  printf("Starting puzzle...\n\033[?25l");
  print_board_subgrid(g);
  printf("\n");
  if (!quick) {
    solved = solve_board(g, hint_board, delay);
  } else {
    solved = solve_board_quick(g, hint_board);
    print_board_subgrid(g);
  }
  if (!solved) {printf("No solution!\n"); exit(EXIT_FAILURE);}
  printf("Solved!\n");
  printf("\033[?25h");
  return 0;
}
