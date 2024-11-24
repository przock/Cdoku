#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "board.h"
#include "solver.h"
#include "flags.h"
#include "cdoku.h"

board game = {0};
board* g = &game;

int **hint_board;
int delay = 200;

bool solved = false;
bool verbose = false;
bool quick = false;
bool custom_delay = false;
bool custom_board = false;

void handle_signal(int signal) {
  if (signal == SIGINT) {
    clean_print_board(g, EXIT_SUCCESS);
  }
}

int main(int argc, char *argv[]) {
  int opt;
  
  while ((opt = getopt(argc, argv, OPTION_STRING)) != -1) {
    switch (opt) {
    case FLAG_JSON_CEM:
      clean(EXIT_FAILURE);
      break;
    case FLAG_DELAY:
      custom_delay = true;
      delay = atoi(optarg);
      break;
    case FLAG_JSON:
      cJSON *board_json = parse_json_cartesian(optarg);
      int **board_pointer = board_from_json_cartesian(board_json);
      
      for (int i = 0; i < 9; i++) {
	for (int j = 0; j < 9; j++) {
	  game[i][j] = board_pointer[i][j];
	}
      }	
      
      custom_board = true;
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
    case FLAG_CLEAN:
      clean(EXIT_SUCCESS);
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

  if (!solved) {
    printf("\033[H\033[J");
    printf("No solution!\n");
    usleep(2000000);
    clean(EXIT_FAILURE);
  }

  printf("Solved!\n");
  printf("\033[?25h");
  return 0;
}
