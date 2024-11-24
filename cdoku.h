#ifndef CDOKU_H
#define CDOKU_H
#include "board.h"
#include <cjson/cJSON.h>

void clean(int code);
void clean_print_board(board *b, int code);
cJSON* parse_json_cartesian(char *filepath);
cJSON* parse_json_cem(char *filepath);

#endif
