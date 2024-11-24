#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <cjson/cJSON.h>
#include "cdoku.h"
#include "board.h"

void clean(int code) {
  printf("\033[H\033[J");
  printf("\033[?25h");
  exit(code);
}

void clean_print_board(board *b, int code) {
  printf("\033[?25h");
  printf("\033[H\033[J");
  print_board_subgrid(b);
  exit(code);
}

cJSON* parse_json_cartesian(char *filepath) {
  char *buffer = NULL;
  long length;
  FILE *f = fopen(filepath, "r");
  
  if (f) {
    fseek(f, 0, SEEK_END);
    length = ftell(f);
    
    if (length < 0) {
      fclose(f);
      return NULL;
    }
    
    fseek(f, 0, SEEK_SET);
    buffer = malloc(length + 1);
    
    if (buffer) {
      size_t read_size = fread(buffer, 1, length, f);
      if (read_size != length) {
	free(buffer);
	fclose(f);
	return NULL;
      }
      buffer[length] = '\0';
    }
    
    fclose(f);
    
    if (!buffer) {
      return NULL;
    } else {
      cJSON* json = cJSON_Parse(buffer);
      free(buffer);
      return json;
    }      
  } else {
    return NULL;
  }
  
  free(buffer);
}

cJSON* parse_json_cem(char *filepath) {

}
