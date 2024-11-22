#ifndef FLAGS_H
#define FLAGS_H
#include <stdbool.h>

#define FLAG_JSON_CEM 'j'
#define FLAG_JSON 'J'
#define FLAG_DELAY 'd'
#define FLAG_HELP 'h'
#define FLAG_VERBOSE 'v'
#define FLAG_QUICK 'q'

#define OPTION_STRING "j:J:d:hvq"
#define HELP_STRING \
"Usage: Cdoku [options]\n" \
"Options:\n" \
"  -j <arg>     Read a board from a .json file in the CEM format\n" \
"  -J <arg>     Read a board from a .json file in Cartesian format\n" \
"  -d <arg>     Specify the delay between write operations to the board, ignored if Quick Mode is enabled\n" \
"  -v           Enable Verbose Mode\n" \
"  -q           Enable Quick Mode, this disables board printing until the puzzle is solved\n" \
"  -h           Print this help message\n" \
"\n" \
"Description:\n" \
"  This program demonstrates the usage of command-line flags using getopt.\n" \
"  The -j and -J options require an argument, and the -h flag displays this help message.\n"

extern bool verbose;

#endif
